from PyQt5.QtWidgets import QApplication, QLabel, QSpinBox, QMainWindow, QRubberBand
from PyQt5.QtGui import QImage, QPixmap


class PyQtView:
    def __init__(self, width, height):
        self.resolution = [width, height]
        self.app = QApplication([])
        self.window = QMainWindow()
        self.label = QLabel()
        self.label.setScaledContents(True)
        self.label.setMinimumSize(192, 108)
        self.selection = QRubberBand(QRubberBand.Rectangle, self.label)
        self.iteration = QSpinBox(self.label)
        self.iteration.setMinimum(1)
        self.iteration.setMaximum(100000)
        self.iteration.setMinimumWidth(50)
        self.window.setCentralWidget(self.label)
        self.window.show()

    def render(self, img):
        qImg = QImage(img, 1920, 1080, 3*1920, QImage.Format_RGB888)
        self.label.setPixmap(QPixmap(qImg))

    def start_selection(self, x, y):
        self.selection.setGeometry(x, y, 1, 1)
        self.selection.show()

    def move_selection(self, x, y):
        width = abs(self.selection.x() - x)
        height = abs(self.selection.y() - y)
        x = min(self.selection.x(), x)
        y = min(self.selection.y(), y)
        self.selection.setGeometry(x, y, width, height)

    def stop_selection(self):
        self.selection.hide()


