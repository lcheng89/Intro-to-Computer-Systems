////////////////////////////////////////////////////////////////////////////////
// Main File:        testN.c (N is 3-bit number) in subdirectory named tests
// This File:        p3Heap.c
// Other Files:      Makefile, p3Heap.h, p3Heap.o, libheap.so, test programs provided in subdirectory named tests
// Semester:         CS 354 Lecture 002 Spring 2025
// Grade Group:      gg09
// Instructor:       Mahmood
// 
// Author:           Leqian Cheng
// Email:            lcheng89@wisc.edu
// CS Login:         leqian
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2025 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes.
// Used with permission, SPRING 2025, Dr. Hina Mahmood
//
/////////////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include "p3Heap.h"

/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block.
 */
typedef struct blockHeader {           

    /*
     * 1) The size of each heap block must be a multiple of 8
     * 2) Heap blocks have blockHeaders that contain size and status bits
     * 3) Free heap blocks contain a footer, but we can use the blockHeader 
     *
     * All heap blocks have a blockHeader with size and status
     * Free heap blocks have a blockHeader as its footer with size only
     *
     * Status is stored using the two least significant bits
     *   Bit0 => least significant bit, last bit
     *   Bit0 == 0 => free block
     *   Bit0 == 1 => allocated block
     *
     *   Bit1 => second last bit 
     *   Bit1 == 0 => previous block is free
     *   Bit1 == 1 => previous block is allocated
     * 
     * Start Heap: 
     *  The blockHeader for the first block of heap is after skipping 4 bytes.
     *  This ensures alignment requirements can be met.
     * 
     * End Mark: 
     *  The end of the available memory is indicated using a size_status of 1.
     * 
     * Examples:
     * 
     * 1. Allocated block of size 24 bytes:
     *    Allocated Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 25
     *      If the previous block is allocated p-bit=1 size_status would be 27
     * 
     * 2. Free block of size 24 bytes:
     *    Free Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 24
     *      If the previous block is allocated p-bit=1 size_status would be 26
     *    Free Block Footer:
     *      size_status should be 24
     */
    int size_status;

} blockHeader;         

/* Global variable - DO NOT CHANGE NAME or TYPE. 
 * It must point to the first block in the heap and is set by init_heap()
 * i.e., the block at the lowest address.
 */
blockHeader *heap_start = NULL;     

/* Size of heap allocation padded to round to the nearest page size.
 */
int alloc_size;

/*
 * Additional global variables may be added as needed below
 * TODO: add global variables as needed by your function
 */




/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block (payload) on success.
 * Returns NULL on failure.
 *
 * This function must:
 * - Check size - Return NULL if size < 1 
 * - Determine block size rounding up to a multiple of 8 
 *   and possibly add padding as a result.
 *
 * - Use BEST-FIT PLACEMENT POLICY to chose a free block
 *
 * - If the BEST-FIT block that is found is exact size match
 *   - 1. Update all heap blocks as needed for any affected blocks
 *   - 2. Return the address of the allocated block payload
 *
 * - If the BEST-FIT block that is found is large enough to split 
 *   - 1. SPLIT the free block into two valid heap blocks:
 *         1. an allocated block
 *         2. a free block
 *         NOTE: both blocks must meet heap block requirements 
 *       - Update all heap block header(s) and footer(s) 
 *              as needed for any affected blocks.
 *   - 2. Return the address of the allocated block payload
 *
 *   Return NULL if unable to find and allocate block of required size
 *
 * Note: payload address that is returned is NOT the address of the
 *       block header. It is the address of the start of the 
 *       available memory for the requester.
 *
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* alloc(int size) {     
    // Check size
    if (size < 1) {
        return NULL;
    }
    
    // Determine block size: header + payload, rounded up to a multiple of 8
    int header_size = sizeof(blockHeader);
    int total_size = header_size + size;
    if (total_size % 8 != 0) {
        total_size = ((total_size + 7) / 8) * 8;
    }
    
    // Set up variables for best-fit search
    blockHeader *current = heap_start;
    blockHeader *best_fit = NULL;
    int best_fit_size = 0;
    
    // Compute the end of the heap region
    char *heap_end = (char*)heap_start + alloc_size;
    
    // Traverse the heap until we hit the end mark or run past the heap
    while (((char*)current < heap_end) && (current->size_status != 1)) {
        int curr_block_size = current->size_status & ~0x3; // clear status bits
        
        // Sanity check: if curr_block_size is non-positive, something is wrong
        if (curr_block_size <= 0) {
            fprintf(stderr, "alloc error: encountered a block with non-positive size (%d).\n", curr_block_size);
            return NULL;
        }
        
        // Check if the block is free (a-bit == 0)
        if ((current->size_status & 1) == 0) {
            // Check if the free block is large enough
            if (curr_block_size >= total_size) {
                // Best-fit: choose the smallest free block that fits
                if (best_fit == NULL || curr_block_size < best_fit_size) {
                    best_fit = current;
                    best_fit_size = curr_block_size;
                }
            }
        }
        // Move to the next block
        current = (blockHeader*)((char*)current + curr_block_size);
    }
    
    // If no suitable free block was found, return NULL
    if (best_fit == NULL) {
        return NULL;
    }
    
    // Decide if we split the chosen free block
    int remaining_size = best_fit_size - total_size;
    if (remaining_size >= 8) {  // Enough room to split.
        // Update the best-fit header: preserve p-bit (bit1) and set a-bit (bit0) to 1
        best_fit->size_status = total_size + (best_fit->size_status & 0x2) + 1;
        
        // Create the new free block immediately after the allocated block
        blockHeader *new_free = (blockHeader*)((char*)best_fit + total_size);
        // Set new free block header: a-bit=0 and p-bit=1
        new_free->size_status = remaining_size + 2;
        
        // Set the footer for the new free block
        blockHeader *new_footer = (blockHeader*)((char*)new_free + remaining_size - 4);
        new_footer->size_status = remaining_size;
        
        // Update the p-bit
        blockHeader *next = (blockHeader*)((char*)new_free + remaining_size);
        if (next->size_status != 1) {  // Not the end mark.
            next->size_status |= 2;     // Set previous block bit.
        }
    }
    else {  // No splitting
        best_fit->size_status = best_fit_size + (best_fit->size_status & 0x2) + 1;
        blockHeader *next = (blockHeader*)((char*)best_fit + best_fit_size);
        if (next->size_status != 1) {
            next->size_status |= 2;
        }
    }
    
    return (void*)(best_fit + 1);
}


/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - Update header(s) and footer as needed.
 *
 * If free results in two or more adjacent free blocks,
 * they will be immediately coalesced into one larger free block.
 * so free blocks require a footer (blockHeader works) to store the size
 *
 * TIP: work on getting immediate coalescing to work after your code 
 *      can pass the tests in partA and partB of tests/ directory.
 *      Submit code that passes partA and partB to Canvas before continuing.
 */                    
int free_block(void *ptr) {    
    //TODO: Your code goes in here.
	// Check if ptr is NULL
    if (ptr == NULL) {
        return -1;
	}

	// Check for 8-byte alignment
    if (((unsigned long)ptr) % 8 != 0) {
        return -1;
    }

	char *heap_begin = (char*)heap_start;
    char *heap_end   = heap_begin + alloc_size;
    char *payload_ptr = (char*)ptr;
    if (payload_ptr < heap_begin + sizeof(blockHeader) || payload_ptr >= heap_end) {
        return -1;
    }

	// Get the block header
    blockHeader *hdr = ((blockHeader*)ptr) - 1;

	// Check if block is already free （a-bit==1)
	if ((hdr->size_status & 1) == 0) {
		return -1;
	}

	int block_size = hdr->size_status & ~0x3;

	//Set a-bit==0
	hdr->size_status = (hdr->size_status & ~1);

    // Write the footer for this free block.
    blockHeader *footer = (blockHeader*)((char*)hdr + block_size - 4);
    footer->size_status = block_size;

    // Coalesce with the next block if it exists and is free.
    blockHeader *next = (blockHeader*)((char*)hdr + block_size);
    if (next->size_status != 1) {  // Not the end mark
        if ((next->size_status & 1) == 0) {  // next block is free
            // Get next block's size.
            int next_size = next->size_status & ~0x3;
            // Increase our block size.
            block_size += next_size;
            // Update current header: preserve our p-bit (bit1) and set new size.
            hdr->size_status = (hdr->size_status & 0x2) | block_size;
            // Update footer of the merged block.
            footer = (blockHeader*)((char*)hdr + block_size - 4);
            footer->size_status = block_size;
            // Update next pointer to the block following the merged block.
            next = (blockHeader*)((char*)hdr + block_size);
		}
		// Update the p-bit of the block
		if (next->size_status != 1) { 
			next->size_status &= ~0x2;
		}
	}
	
	// Coalesce with the previous block if it exists and is free
	if (hdr != heap_start) {
		if ((hdr->size_status & 0x2) == 0) {
			blockHeader *prev_footer = (blockHeader*)((char*)hdr - 4);
			int prev_size = prev_footer->size_status;
			blockHeader *prev_hdr = (blockHeader*)((char*)hdr - prev_size);	
            // Merge the previous block with current block.
            int new_size = prev_size + block_size;
            prev_hdr->size_status = (prev_hdr->size_status & 0x2) | new_size;
            // Update the footer of the merged block.
            footer = (blockHeader*)((char*)prev_hdr + new_size - 4);
            footer->size_status = new_size;
            // Set hdr to the previous block's header (merged block)
            hdr = prev_hdr;
			// Update the next block's p-bit
			next = (blockHeader*)((char*)hdr + new_size);
			if (next->size_status != 1) {
			next->size_status &= ~0x2;
			}
		}
	}
			
    return 0;
} 


/* 
 * Initializes the memory allocator.
 * Called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int init_heap(int sizeOfRegion) {    

    static int allocated_once = 0; //prevent multiple myInit calls

    int   pagesize; // page size
    int   padsize;  // size of padding when heap size is not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int   fd;

    blockHeader* end_mark;

    if (0 != allocated_once) {
        fprintf(stderr, 
                "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }

    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize from O.S. 
    pagesize = getpagesize();

    // Calculate padsize, as padding is required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    alloc_size = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }

    allocated_once = 1;

    // for double word alignment and end mark
    alloc_size -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heap_start = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    end_mark = (blockHeader*)((void*)heap_start + alloc_size);
    end_mark->size_status = 1;

    // Set size in header
    heap_start->size_status = alloc_size;

    // Set p-bit as allocated in header
    // Note a-bit left at 0 for free
    heap_start->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heap_start + alloc_size - 4);
    footer->size_status = alloc_size;

    return 0;
} 

/* STUDENTS MAY EDIT THIS FUNCTION, but do not change function header.
 * TIP: Review this implementation to see one way to traverse through
 *      the blocks in the heap.
 *
 * Can be used for DEBUGGING to help you visualize your heap structure.
 * It traverses heap blocks and prints info about each block found.
 * 
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void disp_heap() {     

    int    counter;
    char   status[6];
    char   p_status[6];
    char * t_begin = NULL;
    char * t_end   = NULL;
    int    t_size;

    blockHeader *current = heap_start;
    counter = 1;

    int used_size =  0;
    int free_size =  0;
    int is_used   = -1;

    fprintf(stdout, 
            "********************************** HEAP: Block List ****************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, 
            "--------------------------------------------------------------------------------\n");

    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;

        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "alloc");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "FREE ");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "alloc");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "FREE ");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;

        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%4i\n", counter, status, 
                p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);

        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, 
            "--------------------------------------------------------------------------------\n");
    fprintf(stdout, 
            "********************************************************************************\n");
    fprintf(stdout, "Total used size = %4d\n", used_size);
    fprintf(stdout, "Total free size = %4d\n", free_size);
    fprintf(stdout, "Total size      = %4d\n", used_size + free_size);
    fprintf(stdout, 
            "********************************************************************************\n");
    fflush(stdout);

    return;  
} 


//		p3Heap.c		END OF FILE                    
                                       
