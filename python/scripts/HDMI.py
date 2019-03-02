import sys

sys.path.append("/home/xilinx/RenderingFractalsOnPYNQ-Z1/python/drivers")
sys.path.append("/home/xilinx/RenderingFractalsOnPYNQ-Z1/python/scripts")

import Mandelbrot
import AxiVDMA_Custom
from Renderer import Renderer

from pynq.lib.video import *
from pynq import Overlay

overlay = Overlay('/home/xilinx/pynq/overlays/personal/Mandelbrot/Mandelbrot.bit')
mandelbrot = overlay.video.mandelbrot
vdma = overlay.video.axi_vdma
hdmi_out = overlay.video.hdmi_out

hdmi_out._vdma = vdma #Quickfix

vdma.readchannel.mode = common.VideoMode(1920, 1080, 32)
hdmi_out.configure(common.VideoMode(1920, 1080, 32))
vdma.readchannel.start()
hdmi_out.start()

renderer = Renderer(mandelbrot, vdma, hdmi_out)
