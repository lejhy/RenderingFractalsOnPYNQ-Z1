from PyQt5.QtWidgets import (
    QApplication,
    QLabel,
    QSpinBox,
    QMainWindow,
    QRubberBand,
    QHBoxLayout,
    QColorDialog,
    QPushButton
)
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
        self.iteration.setMaximum(65535)
        self.iteration.setMinimumWidth(50)
        self.colour_offset = QSpinBox(self.label)
        self.colour_offset.setMinimum(0)
        self.colour_offset.setMaximum(65535)
        self.colour_offset.setMinimumWidth(50)
        self.colour_span = QSpinBox(self.label)
        self.colour_span.setMinimum(1)
        self.colour_span.setMaximum(24)
        self.colour_span.setMinimumWidth(50)
        self.colours = []
        self.save = QPushButton("save", self.label)
        self.demo = QPushButton("demo", self.label)
        self.restart = QPushButton("restart", self.label)
        self.window.setCentralWidget(self.label)
        self.h_box.addWidget(self.iteration)
        self.h_box.addWidget(self.colour_offset)
        self.h_box.addWidget(self.colour_span)
        self.h_box.addWidget(self.save)
        self.h_box.addWidget(self.demo)
        self.h_box.addWidget(self.restart)
        self.window.resize(1920, 1080)
        self.window.show()

    def render(self, img):
        qImg = QImage(img, 1920, 1080, 3*1920, QImage.Format_RGB888)
        self.label.setPixmap(QPixmap(qImg))
        self.app.processEvents()

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

    def create_colour_buttons(self, number, colours):
        for n in range(number):
            button = QPushButton("colour %i" % n, self.label)
            self.colours.append(button)
            button.setFlat(True)
            button.setAutoFillBackground(True)
            self.h_box.addWidget(button)
            self.set_button_colour(button, colours[n])

    def set_button_colour(self, button, colour):
        bg_colour = QColor(colour)
        if bg_colour.lightnessF() < 0.5:
            text_colour = QColor(0xF0F0F0)
        else:
            text_colour = QColor(0x101010)
        palette = button.palette()
        palette.setColor(QPalette.Button, bg_colour)
        palette.setColor(QPalette.ButtonText, text_colour)
        button.setPalette(palette)

    def get_color(self, initial_value):
        Qcolour = QColorDialog.getColor(QColor(initial_value))
        if Qcolour.isValid():
            colour = Qcolour.rgba()
        else:
            colour = initial_value
        return colour


