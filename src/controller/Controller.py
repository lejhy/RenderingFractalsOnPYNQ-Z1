class DesktopController:

    def __init__(self, model, view):
        self.model = model
        self.view = view
        self.update(x_scale=[-2.5, 1], y_scale=[-1, 1], max_iteration=1000)

    def update(self, x_scale, y_scale, max_iteration):
        result = self.model.calculate(x_scale=x_scale, y_scale=y_scale, plot_width=self.view.resolution[0], plot_height=self.view.resolution[1], max_iteration=max_iteration)
        self.view.render(values=result["values"], v_min=result["vMin"], v_max=result["vMax"])
