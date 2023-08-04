image_net_classes = [
"Driving On the Right",
"Slow",
"Speed Limitation"
]

def get_image_net_class(id):
    if id >= len(image_net_classes):
        return "unknown"
    else:
        return image_net_classes[id]