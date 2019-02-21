from pynq.lib.video import *

class AxiVDMA_Custom(pynq.lib.video.dma.AxiVDMA):
    def __init__(self, description):
        super().__init__(description=description, framecount=2)

    bindto = ['xilinx.com:ip:axi_vdma:6.3']