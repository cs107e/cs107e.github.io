---
title: 'Heap allocator diagrams'
---

### Block payload and header

The heap allocator divides the pool of available memory into _blocks_, where each block has a size and status (free or in-use). A `malloc` request is serviced by finding a free block of sufficient size and changing its status from free to in-use. To `free` a block, its status is changed from in-use to free.

The client's data is called the _payload_ of the block. In addition to the space reserved for the payload, the heap allocator also tracks the block metadata (size and status). A convenient place
to store the metadata is in a block header written to the memory immediately preceding the block
payload. This approach is quite efficient
in that simple pointer arithmetic is used to move from the payload to 
the header and back, no search or complex traversal is needed. The per-block header can also be quite small, which means low overhead in terms of space usage.

Consider this struct definition for a block header:

    struct header {
        size_t payload_size;
        int status;       // 0 if free, 1 if in use
    };

`sizeof(struct header)` will require 8 bytes.

Assume our heap starts at address `0x9000` and currently contains just one block of size 0x30 that is free (status = 0).  The heap looks like this (the `x`s indicate payload data, which is whatever the client stored)

Address|Contents|&nbsp;
-------|--------|-----------
[0x9034] | xx xx xx xx | 
[0x9030] | xx xx xx xx | 
[0x902c] | xx xx xx xx | 
[0x9028] | xx xx xx xx | 
[0x9024] | xx xx xx xx | 
[0x9020] | xx xx xx xx | 
[0x901c] | xx xx xx xx | 
[0x9018] | xx xx xx xx | 
[0x9014] | xx xx xx xx | 
[0x9010] | xx xx xx xx | 
[0x900c] | xx xx xx xx | 
[0x9008] | xx xx xx xx | payload starts here
[0x9004] | 00 00 00 00 | hdr.status
[0x9000] | 00 00 00 30 | hdr.payload_size


<style type='text/css'>
table 
{ font-family: Inconsolata, Consolas, Menlo, monospace;
  font-size: 9pt;
  border-collapse: collapse;
  border: none;
}
td:nth-child(2) {
   border-left: 1px solid black; 
   border-right: 1px solid black; 
}
tr td  {
   padding: 1px 10px 1px 10px !important;
}
</style>

<p></p>
If a request for 0x10 bytes comes in, the heap could hand out this large payload as-is, but a more efficient approach would be to split the block in two: 
an in-use block with a payload size of 0x10 and a remainder free block. 
The header for the first block is still at `0x9000`, but its header is
updated to show the payload_size is now 0x10 and status is in-use. 
The header of the remainder block is at address `0x9018`. Its payload
size is 0x18 and its status is freed.  The client who made the request receives the pointer `0x9008`, the address of the payload of the first block.

After the split, the heap now looks like this:

Address|Contents|&nbsp;
-------|--------|-----------
[0x9034] | xx xx xx xx | 
[0x9030] | xx xx xx xx | 
[0x902c] | xx xx xx xx | 
[0x9028] | xx xx xx xx | 
[0x9024] | xx xx xx xx | 
[0x9020] | xx xx xx xx | payload starts here
[0x901c] | 00 00 00 00 | hdr.status
[0x9018] | 00 00 00 18 | hdr.payload_size
[0x9014] | xx xx xx xx | 
[0x9010] | xx xx xx xx | 
[0x900c] | xx xx xx xx | 
[0x9008] | xx xx xx xx | payload starts here
[0x9004] | 00 00 00 01 | hdr.status
[0x9000] | 00 00 00 10 | hdr.payload_size

<p></p>
Given a heap that is laid out with block headers like this, how can you use pointer arithmetic to advance from one header to the subsequent header?   Given the address of the payload, how can you use pointer arithmetic to access the header associated with that payload? (Yes, pointer subtraction is just as legit as pointer addition...)

Pointer arithmetic and proper use of typecasts are all powerful here. Be sure to understand how these mechanisms work and always keep in mind the difference in scaling when adding an offset to an `int *` versus a `char *` versus a `struct header *` and so on.


