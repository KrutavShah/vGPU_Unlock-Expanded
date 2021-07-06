# Here is the community made XenServer script and its instructions:

# Using vgpu5.c

This solution uses the `LD_PRELOAD` method.

## Step 1: Compile

Compile the C file into a shared library.

```
gcc -fPIC -shared -o uvgpu.so vgpu5.c -ldl
```

Take note of the absolute path of this outputted `uvgpu.so` file.

## Step 2: Inject

For KVM, instead of injecting `/opt/vgpu_unlock/vgpu_unlock` before the binary in the SystemD service file, you want to set the environment to have an `LD_PRELOAD`. Before the `ExecStart` line in the SystemD service file, you should add this:

```
Environment=LD_PRELOAD="/path/to/uvgpu.so"
```

Note that you **must** provide an absolute path to the `LD_PRELOAD` `so` file, or else the shared library will not work.

Be sure to do this `Environment` trick in both `nvidia-vgpud.service` and `nvidia-vgpu-mgr.service`.

## Step 3: Profit

Create your MDEVs, and start up your virtual machines as normal, and everything should work!

