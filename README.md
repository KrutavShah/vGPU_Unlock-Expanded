# vGPU_Unlock-Expanded
This repository serves as a collection for community made tools that can unlock vGPU technology on consumer Nvidia GPUs.

Note: I am not the creator of any of these scripts/tools, all credit goes to their original creator. (Should they wish to be credited)

vGPU_Unlock is the original tool made by DualCoder and can be downloaded here. It is the inspiration of these community made spin-offs that improve upon it and add features.

## Contents of this repository
1. Link to the original vgpu_unlock by Dual Coder: https://github.com/DualCoder/vgpu_unlock
2. vGPU Unlock for XenServer: 
3. Improved version of vgpu_unlock built in C: https://gist.github.com/HiFiPhile/b3267ce1e93f15642ce3943db6e60776

Personally, I reccomend trying option 3 if you want an easier solution that's less cluttered and may perform better. It may also work more properly on Linux distributions like Proxmox that don't use Libvirt.

For XenServer installations, you can use the C script included here in this repository. The instructions for it are within the vgpu_unlock-xen folder alongside the script. 

Big thanks to the whole community around vGPU unlocking for coming up with improved solutions to achieving consumer graphics virtualization!
