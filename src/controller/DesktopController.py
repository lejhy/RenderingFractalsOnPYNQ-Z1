
class DesktopController:

    def __init__(self, model, view):
        self.model = model
        self.view = view
        self.update()

    def update(self):
        result = self.model.calculate(plot_width=self.view.resolution[0], plot_height=self.view.resolution[1])
        self.view.render(values=result["values"], v_min=result["vMin"], v_max=result["vMax"])
