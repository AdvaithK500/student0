/*
 * Word count application with one thread per input file.
 *
 * You may modify this file in any way you like, and are expected to modify it.
 * Your solution must read each input file from a separate thread. We encourage
 * you to make as few changes as necessary.
 */

/*
 * Copyright © 2021 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>

#include "word_count.h"
#include "word_helpers.h"

struct thread_args {
    char* filename;
    word_count_list_t* wclist;
};


void* file_thread(void* arg) {
    struct thread_args* args = (struct thread_args*) arg;

    FILE* file = fopen(args->filename, "r");

    if (!file) {
        fprintf(stderr, "Error: could not open file %s\n", args->filename);
        free(arg);
        pthread_exit(NULL);
    }

    count_words(args->wclist, file);
    fclose(file);
    free(arg);  // clean up malloc'd args
    pthread_exit(NULL);
}

/*
 * main - handle command line, spawning one thread per file.
 */
int main(int argc, char* argv[]) {
  /* Create the empty data structure. */
  word_count_list_t word_counts;
  init_words(&word_counts);

  if (argc <= 1) {
    /* Process stdin in a single thread. */
    count_words(&word_counts, stdin);
  } else {
    /* 
    we will have argc - 1 amount of files inputs in the CLI
     */
    pthread_t threads[argc - 1];

    // iterate through and create a thread and apss in the right data for each thread to spawn a seperate file and process it
    for (int i = 1; i < argc; i++) {
      struct thread_args* args = malloc(sizeof(struct thread_args)); // space for taking the args

      args->filename = argv[i]; // argv[1] onwards is the file names to be taken inside the thread args struct
      args->wclist = &word_counts;

      int rc = pthread_create(&threads[i - 1], NULL, file_thread, args);
      if (rc) {
          fprintf(stderr, "Error: failed to create thread for %s\n", argv[i]);
          free(args);
          continue;
      }

    }

    for (int i = 0; i < argc - 1; i++) {
      pthread_join(threads[i], NULL);
    }


  }

  /* Output final result of all threads' work. */
  wordcount_sort(&word_counts, less_count);
  fprint_words(&word_counts, stdout);
  return 0;
}
