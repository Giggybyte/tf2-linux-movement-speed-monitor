# TF2 Movement Speed Monitor

## Summary
Constantly prints your horizontal speed to the terminal. In the future, I'd 
like to overlay this in-game somehow.
[Here's a video of it in use.](https://www.youtube.com/watch?v=9EFNWbolUos)

## Why?
In TF2, there are a few scenarios in which you'd probably like to know your 
horizontal velocity, such as bunnyhopping or rocket jumping. If you try googling
how to do this, you might come across the `cl_showpos` command that you can 
toggle to show your position, view angle, and most importantly, your velocity.

However, you will find that this command isn't as useful as you might expect, 
since as soon as you start jumping, your vertical speed is factored in somehow,
meaning you can't get a clear idea of how fast you're moving horizontally. This
issue exists on Windows as well, but Linux is my primary OS.

## How?
There are two values of interest that I use to calculate my horizontal velocity:
- `client.so+0x218B844` contains a float representing your horizontal velocity
   on axis 1
- `client.so+0x218B848` contains a float representing your horizontal velocity
   on axis 2
In the code, I assume that these two adjacent addresses are the x and y values,
but they could be y and x internally. In any case, it doesn't matter as far as 
the math is concerned. Once I have those two values, the Pythagorean theorem can
be used to calculate the resultant speed.

Reading the memory is done with `process_vm_readv`, which you can find more info
on in the manpages or by googling.

## VAC?
I don't know if you can get VAC'd for using this, but I hope to find out by 
testing on an alternate account soon. During development, I launch tf2 with
`-insecure` to ensure that VAC is disabled. **By downloading and using this, 
you accept this risk, and I am not responsible for anything that happens to your
Steam account.** I would like to think that the chances are small since I'm only
reading from and not writing to memory, but time will tell.

## Install
```
git clone https://github.com/Giggybyte/tf2-linux-movement-speed-monitor/
cd tf2-linux-movement-speed-monitor
gcc -lm -D _GNU_SOURCE -Wall -Wextra tf2-spdmon.c -o tf2-spdmon
sudo ./tf2-spdmon
```
Superuser permissions are required to read the memory of other processes.

## TODO
- Add code comments for anyone stumbling across the repo.
- Figure out an in-game overlay if possible.
