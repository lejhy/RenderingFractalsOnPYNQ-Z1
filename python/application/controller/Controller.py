import sys
import time
import paramiko
import getpass

class Controller:

    def __init__(self, model, view):
        self.model = model
        self.view = view

        #view
        view.label.mousePressEvent = self.handleMousePressEvent
        view.label.mouseMoveEvent = self.handleMouseMoveEvent
        view.label.mouseReleaseEvent = self.handleMouseReleaseEvent
        view.iteration.setValue(model.max_iteration)
        view.iteration.valueChanged.connect(self.handleIterationChange)

        #ssh
        should_connect = input("Connect SSH? (y/n): ")
        if should_connect == "y":
            self.ssh = paramiko.SSHClient()
            self.ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
            default_hostname = "lejhanec.ddns.net"
            default_username = "xilinx"
            hostname = (input("Hostname ({0}): ".format(default_hostname)) or default_hostname)
            username = (input("Username ({0}): ".format(default_username)) or default_username)
            password = getpass.getpass("Password: ", stream=None)
            self.ssh.connect(hostname=hostname, username=username, password=password)
            self.shell = self.ssh.invoke_shell()
            recv = self.shell.recv(1024)
            self.shell.send("su\n")
            while b"Password:" not in recv:
                recv = self.shell.recv(1024)
            self.shell.send(password+"\n")
            self.shell.send("cd RenderingFractalsOnPYNQ-Z1/python/scripts/\n")
            self.shell.send("python3\n")
            self.shell.send("from HDMI import *\n")
            self.model.shell = self.shell

        self.start = {'x': 0, 'y': 0}
        self.end = {'x': 0, 'y': 0}
        self.update()

        sys.exit(view.app.exec_())

    def update(self):
        print(self.shell.recv(1024))
        start = time.time()
        result = self.model.calculate()
        end = time.time()
        print(end - start, " seconds")
        self.view.render(result)

    def updateSelection(self):
        self.model.plot_x_min += min(self.start['x'], self.end['x']) * self.model.plot_width
        self.model.plot_y_max -= min(self.start['y'], self.end['y']) * self.model.plot_height
        self.model.plot_width = abs(self.end['x'] - self.start['x']) * self.model.plot_width
        self.model.plot_height = abs(self.end['y'] - self.start['y']) * self.model.plot_height
        self.update()

    def updateZoomIn(self, x, y):
        self.model.plot_x_min -= (0.25 - x)*self.model.plot_width
        self.model.plot_y_max += (0.25 - y)*self.model.plot_height
        self.model.plot_width /= 2
        self.model.plot_height /= 2
        self.update()

    def updateZoomOut(self, x, y):
        self.model.plot_x_min -= (1.0 - x)*self.model.plot_width
        self.model.plot_y_max += (1.0 - y)*self.model.plot_height
        self.model.plot_width *= 2
        self.model.plot_height *= 2
        self.update()

    def handleMousePressEvent(self, event):
        if event.button() == 1:
            self.start = self.getCoords(event)
            self.view.start_selection(event.localPos().x(), event.localPos().y())
        elif event.button() == 2:
            coords = self.getCoords(event)
            self.updateZoomOut(coords['x'], coords['y'])

    def handleMouseReleaseEvent(self, event):
        if event.button() == 1:
            self.end = self.getCoords(event)
            click_limit = 0.01
            x_diff = abs(self.start['x'] - self.end['x'])
            y_diff = abs(self.start['y'] - self.end['y'])
            if x_diff < click_limit and y_diff < click_limit:
                self.updateZoomIn(self.end['x'], self.end['y'])
            else:
                self.updateSelection()
            self.view.stop_selection()

    def handleMouseMoveEvent(self, event):
        self.view.move_selection(event.localPos().x(), event.localPos().y())

    def handleIterationChange(self, value):
        print(value)
        self.model.max_iteration = value

    def getCoords(self, event):
        pos = event.localPos()
        size = self.view.label.size()
        return {
            'x': pos.x()/size.width(),
            'y': pos.y()/size.height()
        }
