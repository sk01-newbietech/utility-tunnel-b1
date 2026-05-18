cmake_minimum_required(VERSION 3.21)

project(
    utility_tunnel_b1
    LANGUAGES C ASM
    VERSION 0.1.0
    DESCRIPTION "First-person PS1 horror mini demo"
)

psn00bsdk_add_executable(utility_tunnel_b1 GPREL main.c)

psn00bsdk_add_cd_image(
    iso
    utility_tunnel_b1
    iso.xml
    DEPENDS utility_tunnel_b1 system.cnf
)