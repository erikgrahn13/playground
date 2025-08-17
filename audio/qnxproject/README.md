To make QNX Neutrino support Networking in WSL, the following lines needs to be modified in runimage script.

1. Comment out the network_args and set the -netdev to user instead of bridge
# network_args=(-netdev bridge,br=$INTF,id=net0 -device virtio-net-pci,netdev=net0,mac=$OPT_MACADDR)
network_args=(-netdev user,id=net0,hostfwd=tcp::2222-:22 -device virtio-net-pci,netdev=net0,mac=$OPT_MACADDR)


2. Comment out the calling of check-net script
# ${TOOL_DIR}/qemu/check-net $qemu_bin