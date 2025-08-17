To make QNX Neutrino support Networking in WSL, the following lines needs to be modified in runimage script.

1. Comment out the network_args and set the -netdev to user instead of bridge
# network_args=(-netdev bridge,br=$INTF,id=net0 -device virtio-net-pci,netdev=net0,mac=$OPT_MACADDR)
network_args=(-netdev user,id=net0,hostfwd=tcp::2222-:22,hostfwd=tcp::8000-:8000 -device virtio-net-pci,netdev=net0,mac=$OPT_MACADDR)


2. Comment out the calling of check-net script
# ${TOOL_DIR}/qemu/check-net $qemu_bin

Rebuild the image using "Create Virtual target in QNX toolkit"!
Now you should be able to ssh in to the VM with
```bash
$ ssh -m hmac-sha2-512 -p 2222 root@localhost
```

Inside the QNX Toolkit and QNX TARGETS, remove the virtual connection and select "Delete Target Configuration Only"
This will jsut remove the connection to QNX Toolkit but keep the VM on disk.
Instead click "Create QNX Target Connection"
And specify <name>/localhost:8000 where <name> can be anything you want.