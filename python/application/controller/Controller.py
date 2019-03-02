import sys
import time
import paramiko
import getpass

class Controller:

    def __init__(self, model, view):
        self.model = model
        self.view = view

        number_of_colours = 5

        #view
        view.label.mousePressEvent = self.handleMousePressEvent
        view.label.mouseMoveEvent = self.handleMouseMoveEvent
        view.label.mouseReleaseEvent = self.handleMouseReleaseEvent
        view.iteration.setValue(model.max_iteration)
        view.iteration.valueChanged.connect(self.handleIterationChange)
        view.colour_span.setValue(model.colour_span)
        view.colour_span.valueChanged.connect(self.handleColourSpanChange)
        view.colour_offset.setValue(model.colour_offset)
        view.colour_offset.valueChanged.connect(self.handleColourOffsetChange)
        view.create_colour_buttons(number_of_colours, model.colours)
        for n in range(number_of_colours):
            view.colours[n].clicked.connect(self.getColourButtonPressHandler(n))
        view.save.clicked.connect(self.save)

        #ssh
        should_connect = input("Connect SSH? (y/n): ")
        if should_connect == "y":
            self.ssh = paramiko.SSHClient()
            self.ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
            default_hostname = "lejhanec.ddns.net"
            default_username = "xilinx"
            hostname_input = input("Hostname ({0}): ".format(default_hostname))
            username_input = input("Username ({0}): ".format(default_username))
            hostname = hostname_input or default_hostname
            username = username_input or default_username
            password = getpass.getpass("Password: ", stream=None)
            self.ssh.connect(
                hostname=hostname,
                username=username,
                password=password
            )
            self.shell = self.ssh.invoke_shell()
            recv = self.shell.recv(1024)
            self.shell.send("su\n")
            while b"Password:" not in recv:
                recv = self.shell.recv(1024)
            self.shell.send(password+"\n")
            self.shell.send("cd RenderingFractalsOnPYNQ-Z1/python/scripts/\n")
            self.shell.send("python3\n")
            self.shell.send("from HDMI import *\n")
        else:
            self.shell = False

        self.model.shell = self.shell

        self.start = {'x': 0, 'y': 0}
        self.end = {'x': 0, 'y': 0}
        self.update()

        sys.exit(self.exit(view.app.exec_()))

    def save(self, event):
        self.view.label.pixmap().toImage().save("%i.png" % time.time())
        print("save")


    def exit(self, return_code):
        if self.shell:
            self.shell.send("renderer.close()\n")
            print(self.shell.recv(1024))
            self.shell.send("exit()\n")
            print(self.shell.recv(1024))
            self.shell.send("exit\n")
            print(self.shell.recv(1024))
        return return_code

    def update(self):
        if self.shell:
            print(self.shell.recv(1024))
        start = time.time()
        result = self.model.calculate()
        end = time.time()
        print(end - start, " seconds")
        self.view.render(result)

    def updateSelection(self):
        self.model.plot_x_min += self.start['x'] * self.model.plot_width
        self.model.plot_y_max -= self.start['y'] * self.model.plot_height
        self.model.plot_width *= self.end['x'] - self.start['x']
        self.model.plot_height *= self.end['y'] - self.start['y']
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
        coords = self.getCoords(event)
        self.start['x'] = min(self.start['x'], coords['x'])
        self.start['y'] = min(self.start['y'], coords['y'])
        self.view.move_selection(event.localPos().x(), event.localPos().y())

    def handleIterationChange(self, value):
        self.model.max_iteration = value
        self.update()

    def handleColourSpanChange(self, value):
        self.model.colour_span = value
        self.update()

    def handleColourOffsetChange(self, value):
        self.model.colour_offset = value
        self.update()

    def getColourButtonPressHandler(self, n):
        def handler(event):
            colour = self.view.get_color(self.model.get_colour(n))
            self.view.set_button_colour(self.view.colours[n], colour)
            self.model.set_colour(n, colour)
            self.update()
        return handler

    def getCoords(self, event):
        pos = event.localPos()
        size = self.view.label.size()
        return {
            'x': pos.x()/size.width(),
            'y': pos.y()/size.height()
        }
