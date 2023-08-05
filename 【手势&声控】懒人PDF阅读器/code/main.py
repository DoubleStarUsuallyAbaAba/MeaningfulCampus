# 主程序
import sys
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
import fitz
from pyautogui import KEYBOARD_KEYS
from mydialogs import *
from myemail import email_to
from myparser import MyParser
from mythreads import EmailThread, readThread, convertThread, AudioThread
import time
from cnocr import CnOcr
from Vkeyboard import *
from convert import *
from mydatabase import MyDb
from mdEditor import MdEditor
from getCmd import main as getCmd
from Baidu_Text_transAPI import translate
from Gesture_Controller import *

class PDFReader(QMainWindow):

    def __init__(self):
        super(PDFReader, self).__init__()
        self.db = MyDb()  # 数据库
        self.menubar = self.menuBar()
        self.recentfile = None
        self.generateMenuBar()
        self.generateRecentMenu()
        self.recentfile.triggered.connect(self.onRecentFileClicked)  # 此句话修复了bug
        self.toolbar = self.addToolBar("工具栏")
        self.generateToolBar()
        layout = QHBoxLayout(self)
        self.toc = QTreeWidget()
        self.toc.setFont(QFont("", 13))  # 目录文字的字体及其大小控制，修改字体将字体名放入双引号中（为空表示使用默认字体），字体大小修改数字即可，数字越大字体越大
        self.file_path = ""
        self.page_num = 0
        self.currentScrollBarlv = 0
        self.doc = None
        self.book_open = False
        self.note_keeped = ""
        # self.note_loadFromFile = False
        self.note_path = ""
        self.dock = QDockWidget()
        self.generateDockWidget()
        self.addDockWidget(Qt.LeftDockWidgetArea, self.dock)
        self.dock.setVisible(True)
        self.trans_a = 200
        self.trans_b = 200
        self.trans = fitz.Matrix(self.trans_a / 100, self.trans_b / 100).preRotate(0)
        self.scrollarea = QScrollArea(self)
        self.pdfview = QLabel()
        # 添加鼠标事件过滤器
        self.selection_text = ""
        self.selection_png = None
        self.area = None
        self.pdfview.installEventFilter(self)
        self.selection_start = None
        self.selection_end = None
        self.tocDict = {}
        self.scrollarea.setWidget(self.pdfview)
        self.generatePDFView()
        self.editor = MdEditor()
        self.doc2 = QDockWidget()
        self.doc2.setWidget(self.editor)
        self.doc2.setVisible(True)
        self.addDockWidget(Qt.RightDockWidgetArea, self.doc2)
        layout.addWidget(self.scrollarea)
        self.widget = QWidget()
        self.widget.setLayout(layout)
        self.setCentralWidget(self.widget)
        self.setWindowTitle('PDF Reader')

        if len(sys.argv) >= 2:
            myparser = MyParser()
            args = myparser.parser.parse_args()
            if not args.toname and args.filename:
                self.open_file(args.filename)

        desktop = QApplication.desktop()
        rect = desktop.availableGeometry()
        self.screenWidth = rect.width()
        self.screenHeight = rect.height()
        self.setGeometry(rect)
        self.setWindowIcon(QIcon('icon/reader.png'))
        # self.setGeometry(100, 100, 1000, 600)
        self.show()

    # def generatePDFView(self):
    #     if not self.file_path or not self.doc:
    #         return
    #     pix = self.doc[self.page_num].getPixmap(matrix=self.trans)
    #     fmt = QImage.Format_RGBA8888 if pix.alpha else QImage.Format_RGB888
    #     pageImage = QImage(pix.samples, pix.width, pix.height, pix.stride, fmt)
    #     pixmap = QPixmap()
    #     pixmap.convertFromImage(pageImage)
    #     self.pdfview.setPixmap(QPixmap(pixmap))
    #     self.pdfview.resize(pixmap.size())

    
    def generatePDFView(self):
        if not self.file_path or not self.doc:
            return
        pix = self.doc[self.page_num].getPixmap(matrix=self.trans)
        fmt = QImage.Format_RGBA8888 if pix.alpha else QImage.Format_RGB888
        pageImage = QImage(pix.samples, pix.width, pix.height, pix.stride, fmt)
        pixmap = QPixmap()
        pixmap.convertFromImage(pageImage)

        if self.area != None:
            self.draw_selection(pixmap)
            self.area = None
        else:
            self.pdfview.setPixmap(pixmap)
            # print('pixmap.size():', pixmap.size())
            self.pdfview.resize(pixmap.size())
        

    def draw_selection(self, pixmap):
        print("draw_selection..")
        overlay = QPixmap(pixmap.size())
        overlay.fill(Qt.transparent)  # 透明背景

        painter = QPainter(overlay)
        painter.setCompositionMode(QPainter.CompositionMode_Source)
        painter.setOpacity(0.5)  # 设置绘制透明度
        # rect = QRectF(self.area)
        rect = self.area
        painter.fillRect(rect, QColor(0, 0, 255, 128))  # 绘制蓝色半透明矩形
        painter.setCompositionMode(QPainter.CompositionMode_DestinationIn)
        painter.drawPixmap(pixmap.rect(), pixmap)  # 保留选中区域内的原始图像
        painter.end()

        result = QPixmap(pixmap.size())
        result.fill(Qt.transparent)
        painter = QPainter(result)
        painter.drawPixmap(pixmap.rect(), pixmap)  # 绘制原始图像  //
        painter.drawPixmap(pixmap.rect(), overlay)  # 绘制选中区域的蓝色半透明图层
        painter.end()

        self.pdfview.setPixmap(result)
        # print('result.size():', result.size())
        self.pdfview.resize(result.size())

        selection = pixmap.copy(self.area)  # 复制选中区域的图像
        selection.save('temp.png')  # 保存选中区域为图像文件
        self.selection_png = 'temp.png'
        self.ocr()

    def ocr(self):
        print("ocr..")
        ocr = CnOcr()
        res = ocr.ocr(self.selection_png)
        # print("res:", res)
        self.selection_text = ""
        for i in range(len(res)):
            tmp =  str(res[i]['text'])
            # print("res[", i, "]:", tmp + '\n')
            self.selection_text += tmp + '\n'
        print("self.selection_text:", self.selection_text)
        # self.selection_png = None
        
    def eventFilter(self, obj, event):
        if obj is self.pdfview:
            if event.type() ==  QEvent.MouseButtonPress:
                self.selection_start = event.pos()
            elif event.type() ==  QEvent.MouseButtonRelease:
                self.selection_end = event.pos()
                self.extract_selected_text()
        return super().eventFilter(obj, event)

    def extract_selected_text(self):
        if not self.doc:
            return
        
        # 获取选中区域的矩形范围
        rect =  QRect(self.selection_start, self.selection_end).normalized()

        # 获取页面对应的裁剪区域
        page = self.doc[self.page_num]
        clip = fitz.Rect(rect.left(), rect.top(), rect.right(), rect.bottom())

        # 新建一个QRect对象，用于绘制选中区域的矩形框
        self.area = rect

        self.generatePDFView()
        # 提取选中区域的文字
        
        # text = page.getText("text", clip=clip)
        # print("选中文字：", text)



    def generateFile(self):
        file = self.menubar.addMenu('文件')
        file.setFont(QFont("", 13))
        openFile = QAction(QIcon('icon/file.png'), '打开文件', file)
        closeFile = QAction("关闭文件", file)
        saveFile = QAction(QIcon('icon/save.png'), '保存文件', file)
        saveFile.setShortcut('Ctrl+S')

        openFile.triggered.connect(self.onOpen)
        closeFile.triggered.connect(self.onClose)
        saveFile.triggered.connect(self.onSave)

        file.addAction(openFile)
        self.recentfile = file.addMenu('最近文件')
        file.addAction(saveFile)
        file.addAction(closeFile)

    # def generatePage(self):
    #     page = self.menubar.addMenu('页面')
    #     page.setFont(QFont("", 13))
    #     insertPage = QAction(QIcon('icon/insert.png'), '添加页面', page)
    #     deletePage = QAction(QIcon('icon/delete.png'), '删除当前页面', page)
    #     extractPage = QAction(QIcon('icon/pdf.png'), '提取pdf页面', page)
    #
    #     self.pageConnect(deletePage, extractPage, insertPage)
    #
    #     page.addAction(insertPage)
    #     page.addAction(deletePage)
    #     page.addAction(extractPage)

    def generateNote(self):
        note = self.menubar.addMenu('笔记')
        note.setFont(QFont("", 13))
        toNote = QAction("导出笔记", note)
        loadNote = QAction("加载笔记", note)
        keepNote = QAction("保存笔记", note)

        toNote.triggered.connect(self.toNote)
        loadNote.triggered.connect(self.onloadNote)
        keepNote.triggered.connect(self.keepNote)

        note.addAction(toNote)
        note.addAction(loadNote)
        note.addAction(keepNote)

    def generateRecentMenu(self):
        self.recentfile.clear()
        fileList = self.db.getAllRencentFile()
        sortlist = sorted(fileList, key=lambda d: d.opentime, reverse=True)
        for file in sortlist:
            action = QAction(file.path, self.recentfile)
            self.recentfile.addAction(action)
        # self.recentfile.triggered.connect(self.onRecentFileClicked)

    def onRecentFileClicked(self, action):
        self.open_file(action.text())

    def generateInfile(self):
        infile = self.menubar.addMenu('导入')
        infile.setFont(QFont("", 13))
        inHTML = QAction(QIcon('icon/html.png'), '导入HTML', infile)
        inPic = QAction(QIcon('icon/pic.png'), '导入图片', infile)
        inDocx = QAction(QIcon('icon/docx.png'), '导入docx', infile)
        inMarkdown = QAction(QIcon('icon/markdown_2.png'), '导入markdown', infile)

        inHTML.triggered.connect(self.inhtml)
        inPic.triggered.connect(self.inpic)
        inDocx.triggered.connect(self.indocx)
        inMarkdown.triggered.connect(self.inmarkdown)

        infile.addAction(inDocx)
        infile.addAction(inPic)
        infile.addAction(inHTML)
        infile.addAction(inMarkdown)

    def generateOutfile(self):
        outfile = self.menubar.addMenu('导出')
        outfile.setFont(QFont("", 13))
        toToC = QAction('导出目录', outfile)
        toPic = QAction('导出为图片', outfile)
        toHTML = QAction(QIcon('icon/html.png'), '导出为HTML', outfile)
        toTXT = QAction(QIcon('icon/txt.png'), '导出为TXT', outfile)
        toDocx = QAction(QIcon('icon/word.png'), '导出为Docx', outfile)

        toToC.triggered.connect(self.totoc)
        toPic.triggered.connect(self.topic)
        self.tofileConnect(toDocx, toHTML, toTXT)

        outfile.addAction(toToC)
        outfile.addAction(toPic)
        outfile.addSeparator()
        outfile.addAction(toHTML)
        outfile.addAction(toTXT)
        outfile.addAction(toDocx)

    def generateShare(self):
        share = self.menubar.addMenu('分享')
        share.setFont(QFont("", 13))
        # toKindle = QAction(QIcon('icon/kindle.png'), '发送到kindle', share)
        toQQ = QAction(QIcon('icon/QQ.png'), '分享到QQ', share)
        toWechat = QAction(QIcon('icon/wechat.png'), '分享到微信', share)
        # toEmail = QAction(QIcon('icon/email.png'), '通过邮件发送', share)

        # self.shareConnect(toEmail, toKindle, toQQ, toWechat)
        self.shareConnect(toQQ, toWechat)

        #share.addAction(toKindle)
        share.addAction(toQQ)
        share.addAction(toWechat)
        #share.addAction(toEmail)

    def generateMenuBar(self):
        self.menubar.setFont(QFont("", 13))  # 设置菜单栏字体大小
        self.generateFile()
        # self.generatePage()
        self.generateNote()
        self.generateInfile()
        self.generateOutfile()
        self.generateShare()

    def generateToolBar(self):
        self.toolbar.setMinimumSize(QSize(200, 200))
        self.toolbar.setIconSize(QSize(80, 80))  # 设置工具栏图标大小
        # self.toolbar.setToolButtonStyle(Qt.ToolButtonTextBesideIcon)  # 文字在图标旁边
        # self.toolbar.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)  # 文字在图标下方
        # 不设置以上两句话默认只显示图标
        ToC = QAction(QIcon('icon/目录 (5).png'), '目录', self.toolbar)
        openFile = QAction(QIcon('icon/file.png'), '打开文件', self.toolbar)
        saveFile = QAction(QIcon('icon/Save (3).png'), '保存文件', self.toolbar)
        prePage = QAction(QIcon('icon/分页 下一页 (1).png'), '上一页', self.toolbar)
        nextPage = QAction(QIcon('icon/分页 下一页.png'), '下一页', self.toolbar)
        turnPage = QAction(QIcon('icon/跳转.png'), '跳转', self.toolbar)
        # insertPage = QAction(QIcon('icon/insert.png'), '添加页面', self.toolbar)
        # deletePage = QAction(QIcon('icon/delete.png'), '删除当前页面', self.toolbar)
        # extractPage = QAction(QIcon('icon/pdf.png'), '提取pdf页面', self.toolbar)
        enlargePage = QAction(QIcon('icon/放大 (1).png'), '放大', self.toolbar)
        shrinkPage = QAction(QIcon('icon/缩小.png'), '缩小', self.toolbar)
        toHTML = QAction(QIcon('icon/html (3).png'), '导出为HTML', self.toolbar)
        toTXT = QAction(QIcon('icon/txt.png'), '导出为TXT', self.toolbar)
        toDocx = QAction(QIcon('icon/word.png'), '导出为Docx', self.toolbar)
        # toKindle = QAction(QIcon('icon/kindle.png'), '发送到kindle', self.toolbar)
        toQQ = QAction(QIcon('icon/QQ.png'), '分享到QQ', self.toolbar)
        toWechat = QAction(QIcon('icon/wechat.png'), '分享到微信', self.toolbar)
        # toEmail = QAction(QIcon('icon/email.png'), '通过邮件发送', self.toolbar)
        editor = QAction(QIcon('icon/markdown_2.png'), "编辑器", self.toolbar)
        microphone = QAction(QIcon('icon/microphone.png'), "语音命令", self.toolbar)
        camera = QAction(QIcon('icon/camera.png'), "手势控制", self.toolbar)
        editor.triggered.connect(self.onDoc2)

        nextPage.setShortcut(Qt.Key_Right)
        prePage.setShortcut(Qt.Key_Left)

        # self.shareConnect(toEmail, toKindle, toQQ, toWechat)
        openFile.triggered.connect(self.onOpen)
        ToC.triggered.connect(self.onDock)
        saveFile.triggered.connect(self.onSave)
        microphone.triggered.connect(self.onMicrophone)
        camera.triggered.connect(self.onCamera)
        prePage.triggered.connect(self.onPrepage)
        nextPage.triggered.connect(self.nextpage)
        turnPage.triggered.connect(self.turnpage)
        enlargePage.triggered.connect(self.enlargepage)
        shrinkPage.triggered.connect(self.shrinkpage)
        self.tofileConnect(toDocx, toHTML, toTXT)
        # self.pageConnect(deletePage, extractPage, insertPage)

        self.toolbar.addActions([ToC, editor])
        self.toolbar.addSeparator()
        self.toolbar.addActions([openFile, saveFile])
        self.toolbar.addSeparator()
        self.toolbar.addActions([prePage, nextPage, turnPage])
        self.toolbar.addSeparator()
        # self.toolbar.addActions([insertPage, deletePage, extractPage])
        self.toolbar.addSeparator()
        self.toolbar.addActions([enlargePage, shrinkPage])
        self.toolbar.addSeparator()
        self.toolbar.addActions([toHTML, toTXT, toDocx])
        self.toolbar.addSeparator()
        self.toolbar.addActions([toQQ, toWechat])
        self.toolbar.addSeparator()
        self.toolbar.addActions([microphone])
        self.toolbar.addActions([camera])

    def generateDockWidget(self):
        if not self.file_path:
            return
        self.dock.setWidget(self.toc)
        self.generateTreeWidget()

    def generateTreeWidget(self):
        if not self.doc:
            return
        self.toc.setColumnCount(1)
        self.toc.setHeaderLabels(['目录'])
        self.toc.setWindowTitle('目录')
        toc = self.doc.getToC()
        nodelist = [self.toc]
        floorlist = [0]
        tempdict = {}
        if not toc:
            return tempdict
        for line in toc:
            floor, title, page = line
            while floorlist[-1] >= floor:
                nodelist.pop()
                floorlist.pop()
            node = QTreeWidgetItem(nodelist[-1])
            node.setText(0, title)
            nodelist.append(node)
            floorlist.append(floor)
            tempdict[title] = page
        self.tocDict = tempdict
        self.toc.clicked.connect(self.bookmark_jump)

    def bookmark_jump(self, index):
        item = self.toc.currentItem()
        self.page_num = self.tocDict[item.text(0)] - 1
        self.updatePdfView()

    def tofileConnect(self, toDocx, toHTML, toTXT):
        toHTML.triggered.connect(self.tohtml)
        toTXT.triggered.connect(self.totxt)
        toDocx.triggered.connect(self.todocx)

    # def pageConnect(self, deletePage, extractPage, insertPage):
    #     insertPage.triggered.connect(self.insertpage)
    #     deletePage.triggered.connect(self.deletepage)
    #     extractPage.triggered.connect(self.extractpage)
    #
    # def shareConnect(self, toEmail, toKindle, toQQ, toWechat):
    #     toKindle.triggered.connect(self.tokindle)
    #     toQQ.triggered.connect(self.toqq)
    #     toWechat.triggered.connect(self.towechat)
    #     toEmail.triggered.connect(self.toemail)

    def shareConnect(self, toQQ, toWechat):
        toQQ.triggered.connect(self.toqq)
        toWechat.triggered.connect(self.towechat)

    def onDock(self):
        try:
            if self.dock.isVisible():
                self.dock.setVisible(False)
            else:
                self.dock.setVisible(True)
        except AttributeError:
            pass

    def onDoc2(self):
        try:
            if self.doc2.isVisible():
                self.doc2.setVisible(False)
            else:
                self.doc2.setVisible(True)
        except AttributeError:
            pass

    def onOpen(self):
        fDialog = QFileDialog()
        filename, _ = fDialog.getOpenFileName(self, "打开文件", ".", 'PDF file (*.pdf)')
        self.open_file(filename)

    def onMicrophoneResult(self, cmd):
        print('onMicrophoneResult', cmd)
        # QMessageBox.about(self, "语音输入结果", cmd)
        # cmd = '跳转到第328964页'
        if cmd.__contains__('转到'):
            if not self.book_open:
                QMessageBox.about(self, "提示", "请先打开一个文件")
                return
            allpages = self.doc.pageCount
            print(allpages)
            # 取出数字
            page = re.findall(r"\d+\.?\d*", cmd)
            print(page)
            if len(page) == 0:
                QMessageBox.about(self, "提示", "未识别到页数")
                return
            if int(page[0]) > allpages:
                QMessageBox.about(self, "提示", "页数过大")
                return
            elif int(page[0]) <= 0:
                QMessageBox.about(self, "提示", "页数过小")
                return
            # 跳转到指定页面
            self.page_num = int(page[0]) - 1
            self.updatePdfView()
        elif cmd.__contains__('上一页'):
            self.onPrepage()
        elif cmd.__contains__('下一页'):
            self.nextpage()
        elif cmd.__contains__('几点'):
            # 返回当前时刻
            QMessageBox.about(self, "提示", time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
        elif cmd.__contains__('翻译'):
            # 取出翻译内容
            if self.book_open == False:
                QMessageBox.about(self, "提示", "请先打开一个文件")
                return
            elif self.selection_text == "":
                QMessageBox.about(self, "提示", "选中内容为空")
                return
            result = translate('en','zh', self.selection_text) # 英译中结果
            # content = '原文：'+self.selection_text+'\n'+'译文：'+result
            self.editor.appendText(result)
            self.doc2.setVisible(True)

        elif cmd.__contains__('保存'):
            if self.book_open == False:
                QMessageBox.about(self, "提示", "请先打开一个文件")
                return
            elif self.selection_text == "":
                QMessageBox.about(self, "提示", "选中内容为空")
                return
            self.editor.appendText(self.selection_text)
            self.doc2.setVisible(True)
        elif cmd.__contains__('笔记'):
            content = cmd.split('笔记')[1]
            self.editor.appendText(content)
            self.doc2.setVisible(True)
            
    def onMicrophone(self):
        # 执行语音输入
        t = AudioThread(getCmd, ('test',59))
        t.finishSignal.connect(self.onMicrophoneResult)
        t.start()
        time.sleep(0.1)

    def onCamera(self):
        # 执行语音输入
        print('手势控制')

        class GestureController:
            """
            Handles camera, obtain landmarks from mediapipe, entry point
            for whole program.

            Attributes
            ----------
            gc_mode : int
                indicates weather gesture controller is running or not,
                1 if running, otherwise 0.
            cap : Object
                object obtained from cv2, for capturing video frame.
            CAM_HEIGHT : int
                highet in pixels of obtained frame from camera.
            CAM_WIDTH : int
                width in pixels of obtained frame from camera.
            hr_major : Object of 'HandRecog'
                object representing major hand.
            hr_minor : Object of 'HandRecog'
                object representing minor hand.
            dom_hand : bool
                True if right hand is domaniant hand, otherwise False.
                default True.
            """
            gc_mode = 0
            cap = None
            CAM_HEIGHT = None
            CAM_WIDTH = None
            hr_major = None  # Right Hand by default
            hr_minor = None  # Left hand by default
            dom_hand = True

            """
            Executes commands according to detected gestures.

            Attributes
            ----------
            tx_old : int
                previous mouse location x coordinate
            ty_old : int
                previous mouse location y coordinate
            flag : bool
                true if V gesture is detected
            grabflag : bool
                true if FIST gesture is detected
            pinchmajorflag : bool
                true if PINCH gesture is detected through MAJOR hand,
                on x-axis 'Controller.changesystembrightness', 
                on y-axis 'Controller.changesystemvolume'.
            pinchminorflag : bool
                true if PINCH gesture is detected through MINOR hand,
                on x-axis 'Controller.scrollHorizontal', 
                on y-axis 'Controller.scrollVertical'.
            pinchstartxcoord : int
                x coordinate of hand landmark when pinch gesture is started.
            pinchstartycoord : int
                y coordinate of hand landmark when pinch gesture is started.
            pinchdirectionflag : bool
                true if pinch gesture movment is along x-axis,
                otherwise false
            prevpinchlv : int
                stores quantized magnitued of prev pinch gesture displacment, from 
                starting position
            pinchlv : int
                stores quantized magnitued of pinch gesture displacment, from 
                starting position
            framecount : int
                stores no. of frames since 'pinchlv' is updated.
            prev_hand : tuple
                stores (x, y) coordinates of hand in previous frame.
            pinch_threshold : float
                step size for quantization of 'pinchlv'.
            """

            tx_old = 0
            ty_old = 0
            trial = True
            flag = False
            grabflag = False
            pinchmajorflag = False
            pinchminorflag = False
            pinchstartxcoord = None
            pinchstartycoord = None
            pinchdirectionflag = None
            prevpinchlv = 0
            pinchlv = 0
            framecount = 0
            prev_hand = None
            pinch_threshold = 0.3

            def __init__(self):
                """Initilaizes attributes."""
                GestureController.gc_mode = 1
                GestureController.cap = cv2.VideoCapture(0)
                GestureController.CAM_HEIGHT = GestureController.cap.get(cv2.CAP_PROP_FRAME_HEIGHT)
                GestureController.CAM_WIDTH = GestureController.cap.get(cv2.CAP_PROP_FRAME_WIDTH)

            def getpinchylv(hand_result):
                """returns distance beween starting pinch y coord and current hand position y coord."""
                dist = round((GestureController.pinchstartycoord - hand_result.landmark[8].y) * 10, 1)
                return dist

            def getpinchxlv(hand_result):
                """returns distance beween starting pinch x coord and current hand position x coord."""
                dist = round((hand_result.landmark[8].x - GestureController.pinchstartxcoord) * 10, 1)
                return dist

            def changesystembrightness(pdf):
                """sets system brightness based on 'Controller.pinchlv'."""
                # print(type(sbcontrol.get_brightness(display=0)))
                currentBrightnessLv = sbcontrol.get_brightness(display=0) / 100.0
                currentBrightnessLv += GestureController.pinchlv / 25.0
                if currentBrightnessLv > 1.0:
                    currentBrightnessLv = 1.0
                elif currentBrightnessLv < 0.0:
                    currentBrightnessLv = 0.0
                sbcontrol.fade_brightness(int(100 * currentBrightnessLv), start=sbcontrol.get_brightness(display=0))

            def changesystemvolume(pdf):
                """sets system volume based on 'Controller.pinchlv'."""
                currentVolumeLv = GestureController.pinchlv / 5.0
                print('----------------------------------------', currentVolumeLv)
                if currentVolumeLv > 1.0:
                    currentVolumeLv = 1.0
                elif currentVolumeLv < -1.0:
                    currentVolumeLv = -1.0
                else:
                    if currentVolumeLv >= 0.0:
                        pdf.nextpage()
                        print('下一页')
                    else:
                        pdf.onPrepage()
                        print('上一页')
                # volume.SetMasterVolumeLevelScalar(currentVolumeLv, None)

            def scrollVertical(pdf):
                """scrolls on screen vertically."""
                # pyautogui.scroll(120 if GestureController.pinchlv > 0.0 else -120)
                currentVolumeLv = GestureController.pinchlv / 50.0
                if currentVolumeLv > 0.1:
                    currentVolumeLv = 0.1
                elif currentVolumeLv < -0.1:
                    currentVolumeLv = -0.1
                pdf.currentScrollBarlv += (-1.0) * currentVolumeLv
                if pdf.currentScrollBarlv > 1.0:
                    pdf.currentScrollBarlv = 1.0
                elif pdf.currentScrollBarlv < 0.0:
                    pdf.currentScrollBarlv = 0.0
                print('----------------------------------------', pdf.currentScrollBarlv)
                pdf.scrollarea.verticalScrollBar().setValue(
                    pdf.currentScrollBarlv * pdf.scrollarea.verticalScrollBar().maximum())
                # print(pdf.scrollarea.verticalScrollBar().maximum())
                # print(pdf.scrollarea.verticalScrollBar().minimum())
                time.sleep(0.5)

            def scrollHorizontal(pdf):
                """scrolls on screen horizontally."""
                # pyautogui.keyDown('shift')
                # pyautogui.keyDown('ctrl')
                # pyautogui.scroll(-120 if GestureController.pinchlv > 0.0 else 120)
                # pyautogui.keyUp('ctrl')
                # pyautogui.keyUp('shift')

                currentVolumeLv = GestureController.pinchlv / 5.0
                print('----------------------------------------', currentVolumeLv)
                if currentVolumeLv > 1.0:
                    currentVolumeLv = 1.0
                elif currentVolumeLv < -1.0:
                    currentVolumeLv = -1.0
                else:
                    if currentVolumeLv >= 0.0:
                        pdf.enlargepage()
                        print('放大页面')
                    else:
                        pdf.shrinkpage()
                        print('缩小页面页')
                # volume.SetMasterVolumeLevelScalar(currentVolumeLv, None)

            # Locate Hand to get Cursor Position
            # Stabilize cursor by Dampening
            def get_position(hand_result):
                """
                returns coordinates of current hand position.

                Locates hand to get cursor position also stabilize cursor by
                dampening jerky motion of hand.

                Returns
                -------
                tuple(float, float)
                """
                point = 9
                position = [hand_result.landmark[point].x, hand_result.landmark[point].y]
                sx, sy = pyautogui.size()
                x_old, y_old = pyautogui.position()
                x = int(position[0] * sx)
                y = int(position[1] * sy)
                if GestureController.prev_hand is None:
                    GestureController.prev_hand = x, y
                delta_x = x - GestureController.prev_hand[0]
                delta_y = y - GestureController.prev_hand[1]

                distsq = delta_x ** 2 + delta_y ** 2
                ratio = 1
                GestureController.prev_hand = [x, y]

                if distsq <= 25:
                    ratio = 0
                elif distsq <= 900:
                    ratio = 0.07 * (distsq ** (1 / 2))
                else:
                    ratio = 2.1
                x, y = x_old + delta_x * ratio, y_old + delta_y * ratio
                return (x, y)

            def pinch_control_init(hand_result):
                """Initializes attributes for pinch gesture."""
                GestureController.pinchstartxcoord = hand_result.landmark[8].x
                GestureController.pinchstartycoord = hand_result.landmark[8].y
                GestureController.pinchlv = 0
                GestureController.prevpinchlv = 0
                GestureController.framecount = 0

            # Hold final position for 5 frames to change status
            def pinch_control(hand_result, controlHorizontal, controlVertical, pdf):
                """
                calls 'controlHorizontal' or 'controlVertical' based on pinch flags,
                'framecount' and sets 'pinchlv'.

                Parameters
                ----------
                hand_result : Object
                    Landmarks obtained from mediapipe.
                controlHorizontal : callback function assosiated with horizontal
                    pinch gesture.
                controlVertical : callback function assosiated with vertical
                    pinch gesture.

                Returns
                -------
                None
                """
                if GestureController.framecount == 5:
                    GestureController.framecount = 0
                    GestureController.pinchlv = GestureController.prevpinchlv

                    if GestureController.pinchdirectionflag == True:
                        controlHorizontal(pdf)  # x

                    elif GestureController.pinchdirectionflag == False:
                        controlVertical(pdf)  # y

                lvx = GestureController.getpinchxlv(hand_result)
                lvy = GestureController.getpinchylv(hand_result)

                if abs(lvy) > abs(lvx) and abs(lvy) > GestureController.pinch_threshold:
                    GestureController.pinchdirectionflag = False
                    if abs(GestureController.prevpinchlv - lvy) < GestureController.pinch_threshold:
                        GestureController.framecount += 1
                    else:
                        GestureController.prevpinchlv = lvy
                        GestureController.framecount = 0

                elif abs(lvx) > GestureController.pinch_threshold:
                    GestureController.pinchdirectionflag = True
                    if abs(GestureController.prevpinchlv - lvx) < GestureController.pinch_threshold:
                        GestureController.framecount += 1
                    else:
                        GestureController.prevpinchlv = lvx
                        GestureController.framecount = 0

            def handle_controls(gesture, hand_result, pdf):
                """Impliments all gesture functionality."""
                x, y = None, None
                if gesture != Gest.PALM:
                    x, y = GestureController.get_position(hand_result)

                # flag reset
                if gesture != Gest.FIST and GestureController.grabflag:
                    GestureController.grabflag = False
                    pyautogui.mouseUp(button="left")

                if gesture != Gest.PINCH_MAJOR and GestureController.pinchmajorflag:
                    GestureController.pinchmajorflag = False

                if gesture != Gest.PINCH_MINOR and GestureController.pinchminorflag:
                    GestureController.pinchminorflag = False

                # implementation
                if gesture == Gest.V_GEST:
                    GestureController.flag = True
                    pyautogui.moveTo(x, y, duration=0.1)

                elif gesture == Gest.FIST:
                    if not GestureController.grabflag:
                        GestureController.grabflag = True
                    #     pyautogui.mouseDown(button="left")
                    # pyautogui.moveTo(x, y, duration=0.1)
                    # print('close--close--close--close--close--close--close--close')
                    GestureController.gc_mode = 0

                elif gesture == Gest.MID and GestureController.flag:
                    pyautogui.click()
                    GestureController.flag = False

                elif gesture == Gest.INDEX and GestureController.flag:
                    # pyautogui.click(button='right')
                    # GestureController.flag = False
                    print('打开语音')
                    pdf.onMicrophone()
                    GestureController.flag = False

                elif gesture == Gest.TWO_FINGER_CLOSED and GestureController.flag:
                    pyautogui.doubleClick()
                    GestureController.flag = False

                elif gesture == Gest.PINCH_MINOR:
                    if GestureController.pinchminorflag == False:
                        GestureController.pinch_control_init(hand_result)
                        GestureController.pinchminorflag = True
                    GestureController.pinch_control(hand_result, GestureController.scrollHorizontal,
                                                    GestureController.scrollVertical, pdf)

                elif gesture == Gest.PINCH_MAJOR:
                    if GestureController.pinchmajorflag == False:
                        GestureController.pinch_control_init(hand_result)
                        GestureController.pinchmajorflag = True
                    GestureController.pinch_control(hand_result, GestureController.changesystembrightness,
                                                    GestureController.changesystemvolume, pdf)

            def classify_hands(results):
                """
                sets 'hr_major', 'hr_minor' based on classification(left, right) of
                hand obtained from mediapipe, uses 'dom_hand' to decide major and
                minor hand.
                """
                left, right = None, None
                try:
                    handedness_dict = MessageToDict(results.multi_handedness[0])
                    if handedness_dict['classification'][0]['label'] == 'Right':
                        right = results.multi_hand_landmarks[0]
                    else:
                        left = results.multi_hand_landmarks[0]
                except:
                    pass

                try:
                    handedness_dict = MessageToDict(results.multi_handedness[1])
                    if handedness_dict['classification'][0]['label'] == 'Right':
                        right = results.multi_hand_landmarks[1]
                    else:
                        left = results.multi_hand_landmarks[1]
                except:
                    pass

                if GestureController.dom_hand == True:
                    GestureController.hr_major = right
                    GestureController.hr_minor = left
                else:
                    GestureController.hr_major = left
                    GestureController.hr_minor = right

            def start(self, pdf):
                """
                Entry point of whole programm, caputres video frame and passes, obtains
                landmark from mediapipe and passes it to 'handmajor' and 'handminor' for
                controlling.
                """

                handmajor = HandRecog(HLabel.MAJOR)
                handminor = HandRecog(HLabel.MINOR)

                with mp_hands.Hands(max_num_hands=2, min_detection_confidence=0.5,
                                    min_tracking_confidence=0.5) as hands:
                    while GestureController.cap.isOpened() and GestureController.gc_mode:
                        success, image = GestureController.cap.read()
                        

                        if not success:
                            print("Ignoring empty camera frame.")
                            continue
                        img_height, img_width, img_channels = image.shape
                        image = cv2.cvtColor(cv2.flip(image, 1), cv2.COLOR_BGR2RGB)
                        image.flags.writeable = False
                        results = hands.process(image)

                        image.flags.writeable = True
                        image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)

                        if results.multi_hand_landmarks:
                            GestureController.classify_hands(results)
                            handmajor.update_hand_result(GestureController.hr_major)
                            handminor.update_hand_result(GestureController.hr_minor)

                            handmajor.set_finger_state()
                            handminor.set_finger_state()
                            gest_name = handminor.get_gesture()

                            if gest_name == Gest.PINCH_MINOR:
                                GestureController.handle_controls(gest_name, handminor.hand_result, pdf)
                            else:
                                gest_name = handmajor.get_gesture()
                                GestureController.handle_controls(gest_name, handmajor.hand_result, pdf)

                            for hand_landmarks in results.multi_hand_landmarks:
                                mp_drawing.draw_landmarks(image, hand_landmarks, mp_hands.HAND_CONNECTIONS)
                        else:
                            GestureController.prev_hand = None

                        window_name = 'Gesture Controller'
                        cv2.namedWindow(window_name, cv2.WINDOW_NORMAL)

                        cv2.resizeWindow(window_name, int(img_width * 0.75), int(img_height * 0.75))  # 设置窗口大小为800x600
                        gesture_window_x = pdf.screenWidth - int(img_width * 0.75) - 1
                        gesture_window_y = pdf.screenHeight - int(img_height * 0.75) - 40
                        cv2.moveWindow(window_name, gesture_window_x, gesture_window_y)
                        cv2.imshow(window_name, image)
                        if cv2.waitKey(1) == 27:
                            break
                GestureController.cap.release()
                cv2.destroyAllWindows()

        gc1 = GestureController()
        gc1.start(self)

    def open_file(self, filename):
        if not filename:
            return
        if not self.db.fileInDB(filename):
            self.db.addRecentFile(filename)
        else:
            self.db.updateRecentFile(filename)
        if os.path.exists(filename):
            self.file_path = filename
        else:
            QMessageBox.about(self, "提醒", "文件不存在")
            self.db.deleteRecentFile(filename)
        self.toc.clear()
        self.page_num = 0
        self.book_open = True
        self.generateRecentMenu()
        self.getDoc()
        self.generateDockWidget()
        self.generatePDFView()

    def getDoc(self):
        if self.file_path:
            self.doc = fitz.open(self.file_path)

    def onClose(self):
        self.file_path = ""
        self.book_open = False
        self.toc.clear()
        self.pdfview.clear()
        self.getDoc()
        self.generatePDFView()
        self.generateDockWidget()

    def onSave(self):
        if not self.book_open:
            return
        self.doc.save(self.doc.name, incremental=True, encryption=fitz.PDF_ENCRYPT_KEEP)

    def onPrepage(self):
        if self.book_open == False:
            QMessageBox.about(self, "提示", "请先打开一个文件")
            return
        self.page_num -= 1
        if self.page_num < 0:
            self.page_num += self.doc.pageCount
        self.updatePdfView()
        time.sleep(0.5)

    def updatePdfView(self):
        self.scrollarea.verticalScrollBar().setValue(0)
        # print(self.scrollarea.verticalScrollBar().maximum())
        # print(self.scrollarea.verticalScrollBar().minimum())
        self.generatePDFView()

    def nextpage(self):
        if self.book_open == False:
            QMessageBox.about(self, "提示", "请先打开一个文件")
            return
        self.page_num += 1
        if self.page_num >= self.doc.pageCount:
            self.page_num -= self.doc.pageCount
        self.updatePdfView()
        time.sleep(0.5)

    def turnpage(self):
        if not self.book_open:
            return
        allpages = self.doc.pageCount
        print(allpages)
        page, ok = QInputDialog.getInt(self, "选择页面", "输入目标页面({}-{})".format(1, allpages), min=1, max=allpages)
        if ok:
            self.page_num = page - 1
            self.updatePdfView()


    def enlargepage(self):
        if self.book_open == False:
            QMessageBox.about(self, "提示", "请先打开一个文件")
            return
    
        self.trans_a += 5  # 每次放大增加5%,修改此参数时注意和下方的tran_b保持一致，否则图片会变形
        self.trans_b += 5
        self.trans = fitz.Matrix(self.trans_a / 100, self.trans_b / 100).preRotate(0)
        self.generatePDFView()
        time.sleep(0.5)

    def shrinkpage(self):
        if self.book_open == False:
            QMessageBox.about(self, "提示", "请先打开一个文件")
            return
        self.trans_a -= 5
        self.trans_b -= 5
        self.trans = fitz.Matrix(self.trans_a / 100, self.trans_b / 100).preRotate(0)
        self.generatePDFView()
        time.sleep(0.5)

    def insertpage(self):
        if not self.book_open:
            return
        dig = InsertDialog(self)
        dig.picSignal.connect(self.insertpage_pic)
        dig.pdfSignal.connect(self.insertpage_pdf)
        dig.show()

    def insertpage_pic(self, pic_list):
        if not self.book_open:
            return
        for pic in pic_list:
            img = fitz.open(pic)
            rect = img[0].rect  # pic dimension
            pdfbytes = img.convertToPDF()  # make a PDF stream
            img.close()  # no longer needed
            imgPDF = fitz.open("pdf", pdfbytes)  # open stream as PDF
            page = self.doc.newPage(self.page_num, width=rect.width,  # new page with ...
                                    height=rect.height)  # pic dimension
            page.showPDFpage(rect, imgPDF, 0)  # image fills the page
        self.generatePDFView()

    def insertpage_pdf(self, file_path, start, end):
        if not self.book_open:
            return
        doc2 = fitz.open(file_path)
        self.doc.insertPDF(doc2, from_page=start - 1, to_page=end - 1, start_at=self.page_num)
        self.generatePDFView()

    def deletepage(self):
        if not self.book_open:
            return
        self.doc.deletePage(self.page_num)
        self.updatePdfView()

    def extractpage(self):
        if not self.book_open:
            return
        allpages = self.doc.pageCount
        start, ok = QInputDialog.getInt(self, "选择开始页面", "输入开始页面(1-{})".format(allpages), min=1, max=allpages)
        if ok:
            end, ok = QInputDialog.getInt(self, "选择结束页面", "输入结束页面({}-{})".format(start, allpages), min=start,
                                          max=allpages)
            if ok:
                file_name, _ = QFileDialog.getSaveFileName(self, "保存文件", ".", "PDF File(*.pdf)")
                if file_name:
                    doc2 = fitz.open()
                    doc2.insertPDF(self.doc, from_page=start - 1, to_page=end - 1)
                    doc2.save(file_name)

    def inhtml(self):
        dig = inHtmlDialog(self)
        dig.finishSignal.connect(self.open_file)
        dig.show()

    def inmarkdown(self):
        filename, _ = QFileDialog.getOpenFileName(self, "选择文件", ".", "markdown file(*.md)")
        if filename:
            toname, _ = QFileDialog.getSaveFileName(self, "保存文件", ".", "pdf file(*.pdf)")
            if toname:
                t = convertThread(mdToPdf, (filename, toname))
                t.finishSignal.connect(lambda: self.open_file(toname))
                t.start()
                time.sleep(1)

    def indocx(self):
        filename, _ = QFileDialog.getOpenFileName(self, "选择文件", ".", "docx file(*.docx)")
        if filename:
            toname, _ = QFileDialog.getSaveFileName(self, "保存文件", ".", "pdf file(*.pdf)")
            if toname:
                docxToPdf(filename, toname)
                self.open_file(toname)

    def inpic(self):
        dig = InPicDialog(self)
        dig.finishSignal.connect(self.open_file)
        dig.show()

    def tohtml(self):
        if not self.book_open:
            return
        toname, _ = QFileDialog.getSaveFileName(self, "保存文件", ".", "html file(*.html)")
        if toname:
            t = convertThread(pdfToHtmlorTxt, (self.file_path, toname, "html"))
            t.finishSignal.connect(lambda: self.openFileNote(toname))
            t.start()
            time.sleep(1)

    def totxt(self):
        if not self.book_open:
            return
        toname, _ = QFileDialog.getSaveFileName(self, "保存文件", ".", "txt file(*.txt)")
        if toname:
            t = convertThread(pdfToHtmlorTxt, (self.file_path, toname, "text"))
            t.finishSignal.connect(lambda: self.openFileNote(toname))
            t.start()
            time.sleep(1)

    def totoc(self):
        if not self.book_open:
            return
        toname, _ = QFileDialog.getSaveFileName(self, "保存文件", ".", "markdown file(*.md)")
        if toname:
            t = convertThread(tocToMd, (self.file_path, toname))
            t.finishSignal.connect(lambda: self.openFileNote(toname))
            t.start()
            time.sleep(1)

    def openFileNote(self, file_path):
        ret = QMessageBox.question(self, '提示', "转换成功，是否打开文件", QMessageBox.Yes | QMessageBox.No, QMessageBox.Yes)
        if ret == QMessageBox.Yes:
            os.startfile(file_path)

    def toNote(self):
        file, _ = QFileDialog.getSaveFileName(self, "导出笔记", ".", "markdown file(*.md)")
        if file:
            text = self.editor.writePart.toPlainText()
            self.note_keeped = text
            # self.note_loadFromFile = True
            self.note_path = file
            t = convertThread(writeToFile, (file, text))
            t.start()
            time.sleep(0.1)

    def onloadNote(self):
        file, _ = QFileDialog.getOpenFileName(self, "导入笔记", ".", "markdown file(*.md)")
        if file:
            t = readThread(readFromFile, file)
            t.finishSignal.connect(self.finishRead)
            self.note_path = file
            t.start()
            time.sleep(0.1)

    def finishRead(self, text):
        self.editor.writePart.setPlainText(text)
        # self.note_loadFromFile = True
        self.doc2.setVisible(True)
        self.note_keeped = text

    def keepNote(self):
        if os.path.exists(self.note_path):  # 已保存过的存在文件
            text = self.editor.writePart.toPlainText()
            self.note_keeped = text
            t = convertThread(writeToFile, (self.note_path, text))
            t.start()
            time.sleep(0.1)
        else:
            self.toNote()

    def topic(self):
        if not self.book_open:
            return
        file = QFileDialog.getExistingDirectory(self, "", ".")
        if file:
            y = convertThread(pdfToImg, (self.file_path, file))
            y.start()
            time.sleep(1)

    def todocx(self):
        if not self.book_open:
            return
        toname, _ = QFileDialog.getSaveFileName(self, "保存文件", ".", "docx file(*.docx)")
        if toname:
            t = convertThread(pdfToDocx, (self.file_path, toname))
            t.finishSignal.connect(lambda: self.openFileNote(toname))
            t.start()
            time.sleep(1)

    def tokindle(self):
        if not self.book_open:
            return
        emailList = self.db.getAllKindleMail()
        dig = EmailToKindleDialog(self, emailList)
        dig.addressSignal.connect(self.sendMail)
        dig.show()

    def sendMail(self, address):
        if not self.db.mailInDB(address):
            self.db.addKindleMail(address)
        t = EmailThread(email_to, (self.file_path, address))
        t.finishSignal.connect(self.finish_mail)
        t.start()
        time.sleep(0.8)

    def finish_mail(self, success):
        if success:
            QMessageBox.about(self, "提示", "发送成功")
        else:
            QMessageBox.about(self, "提示", "抱歉，发送失败，请检查邮箱后再次发送")

    def toqq(self):
        if not self.book_open:
            return
        copyFile(self.file_path)
        QMessageBox.about(self, "提示", "文件已复制到剪贴板")
        CtrlAltZ()

    def towechat(self):
        copyFile(self.file_path)
        QMessageBox.about(self, "提示", "文件已复制到剪贴板")
        CtrlAltW()

    def toemail(self):
        if not self.book_open:
            return
        dig = EmailToOthersDialog(self, self.file_path)
        dig.emailSignal.connect(self.onToemail)
        dig.show()

    def onToemail(self, suc, fail):
        QMessageBox.about(self, "提示", "{}个成功，{}个失败".format(suc, fail))

    def closeEvent(self, *args, **kwargs):
        text = self.editor.writePart.toPlainText()
        if text:
            if text != self.note_keeped:
                ret = QMessageBox.question(self, '提示', "笔记尚未保存，是否保存", QMessageBox.Yes | QMessageBox.No, QMessageBox.Yes)
                if ret == QMessageBox.Yes:
                    self.keepNote()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    reader = PDFReader()
    sys.exit(app.exec_())
