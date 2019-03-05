# HDMI Signals
set_property -dict {
    PACKAGE_PIN L17 IOSTANDARD TMDS_33} [get_ports hdmi_out_clk_n]
set_property -dict {
    PACKAGE_PIN L16 IOSTANDARD TMDS_33} [get_ports hdmi_out_clk_p]
set_property -dict {
    PACKAGE_PIN K18 IOSTANDARD TMDS_33} [get_ports {hdmi_out_data_n[0]}]
set_property -dict {
    PACKAGE_PIN K17 IOSTANDARD TMDS_33} [get_ports {hdmi_out_data_p[0]}]
set_property -dict {
    PACKAGE_PIN J19 IOSTANDARD TMDS_33} [get_ports {hdmi_out_data_n[1]}]
set_property -dict {
    PACKAGE_PIN K19 IOSTANDARD TMDS_33} [get_ports {hdmi_out_data_p[1]}]
set_property -dict {
    PACKAGE_PIN H18 IOSTANDARD TMDS_33} [get_ports {hdmi_out_data_n[2]}]
set_property -dict {
    PACKAGE_PIN J18 IOSTANDARD TMDS_33} [get_ports {hdmi_out_data_p[2]}]
set_property -dict {
    PACKAGE_PIN R19 IOSTANDARD LVCMOS33} [get_ports {hdmi_out_hpd[0]}]
set_property -dict {
    PACKAGE_PIN M17 IOSTANDARD LVCMOS33} [get_ports hdmi_out_ddc_scl_io]
set_property -dict {
    PACKAGE_PIN M18 IOSTANDARD LVCMOS33} [get_ports hdmi_out_ddc_sda_io]