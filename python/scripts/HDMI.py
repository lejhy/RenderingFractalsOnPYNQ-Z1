import Mandelbrot
import AxiVDMA_Custom

from pynq.lib.video import *
from pynq import Overlay

overlay = Overlay('/home/xilinx/pynq/overlays/personal/Mandelbrot/Mandelbrot.bit')
mandelbrot = overlay.video.mandelbrot
vdma = overlay.video.axi_vdma
hdmi_out = overlay.video.hdmi_out

hdmi_out._vdma = vdma #Find out where is this coming from, possibly the class override?

vdma.readchannel.mode = common.VideoMode(1920, 1080, 32)
hdmi_out.configure(common.VideoMode(1920, 1080, 32))
vdma.readchannel.start()
hdmi_out.start()
vdma.readchannel.tie(vdma.writechannel)
