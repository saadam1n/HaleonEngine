# TODO List
This is the TODO list:

- Add Vulkan backend
- Refactor the other backends, especially Direct3D 12. This probably is going to be making working with DXGI easier by creating a new class that cares of the swap chain
- Fix live unrefrenced objects in the Direct3D12 backend - DONE
- Start working on audio component. I also need to choose whether to use libsoundio or RtAudio, but I think RtAudio is better for my C++ engine