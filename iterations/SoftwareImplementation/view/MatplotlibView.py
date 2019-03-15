import matplotlib.pyplot as plt


class MatplotlibView:

    def __init__(self, resolutionX, resolutionY):
        self.resolution = [resolutionX, resolutionY]
        self.dpi = 300
        self.size = [resolutionX/self.dpi, resolutionY/self.dpi]
        plt.figure(figsize=self.size, dpi=self.dpi)

    def render(self, values, v_min, v_max):
        plt.imshow(
            values,
            "gist_stern",
            vmin=v_min,
            vmax=v_max,
            extent=(-2.5, 1.0, -1.0, 1.0)
        )
        plt.show()
