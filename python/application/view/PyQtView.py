from PyQt5.QtWidgets import QApplication, QLabel, QSpinBox, QMainWindow, QRubberBand, QHBoxLayout, QColorDialog, QPushButton
from PyQt5.QtGui import QImage, QPixmap, QColor, QPalette
from PyQt5.QtCore import Qt


class PyQtView:
    def __init__(self, width, height):
        self.resolution = [width, height]
        self.app = QApplication([])
        self.window = QMainWindow()
        self.label = QLabel()
        self.label.setScaledContents(True)
        self.label.setMinimumSize(192, 108)
        self.selection = QRubberBand(QRubberBand.Rectangle, self.label)
        self.h_box = QHBoxLayout(self.label)
        self.h_box.setAlignment(Qt.AlignTop)
        self.iteration = QSpinBox(self.label)
        self.iteration.setMinimum(1)
        self.iteration.setMaximum(100000)
        self.iteration.setMinimumWidth(50)
        self.colour_span = QSpinBox(self.label)
        self.colour_span.setMinimum(1)
        self.colour_span.setMaximum(24)
        self.colour_span.setMinimumWidth(50)
        self.colour_0 = QPushButton("colour_0", self.label)
        self.colour_0.setFlat(True)
        self.colour_0.setAutoFillBackground(True)
        self.colour_1 = QPushButton("colour_1", self.label)
        self.colour_1.setFlat(True)
        self.colour_1.setAutoFillBackground(True)
        self.colour_2 = QPushButton("colour_2", self.label)
        self.colour_2.setFlat(True)
        self.colour_2.setAutoFillBackground(True)
        self.colour_3 = QPushButton("colour_3", self.label)
        self.colour_3.setFlat(True)
        self.colour_3.setAutoFillBackground(True)
        self.window.setCentralWidget(self.label)
        self.h_box.addWidget(self.iteration)
        self.h_box.addWidget(self.colour_span)
        self.h_box.addWidget(self.colour_0)
        self.h_box.addWidget(self.colour_1)
        self.h_box.addWidget(self.colour_2)
        self.h_box.addWidget(self.colour_3)
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

    def colour_button(self, button, colour_value):
        bg_colour = QColor(colour_value)
        if bg_colour.lightnessF() < 0.5:
            text_colour = QColor(0xF0F0F0)
        else:
            text_colour = QColor(0x101010)
        palette = button.palette()
        palette.setColor(QPalette.Button, bg_colour)
        palette.setColor(QPalette.ButtonText, text_colour)
        button.setPalette(palette)

    def get_color(self, initial_value):
        return QColorDialog.getColor(QColor(initial_value)).rgba()


