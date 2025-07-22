/*
 * Implementation of the word_count interface using Pintos lists.
 *
 * You may modify this file, and are expected to modify it.
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

#ifndef PINTOS_LIST
#error "PINTOS_LIST must be #define'd when compiling word_count_l.c"
#endif

#include "word_count.h"

void init_words(word_count_list_t* wclist) {
  // takes a pointer to the word count list - typedef struct list word_count_list_t;
  /* 
          typedef struct word_count {
        char* word;
        int count;
        struct list_elem elem;
      } word_count_t;
  */

 /*
struct list {
  struct list_elem head; 
  struct list_elem tail; 

*/

/** 
 * 
 * struct list_elem {
  struct list_elem* prev; 
  struct list_elem* next; 
};
 */

// so word_count_list, is an alias for the list type as there is no pthreads defined at this stage
  // IDEA: initialise the list using 
  // list_init(struct list*);
  list_init(wclist);
}



size_t len_words(word_count_list_t* wclist) {
  int len = 0;
  /*
    idea: iterate through using wclist until u reach the null pointer, iterate using the list API
  */

   struct list_elem *e;

  for (e = list_begin (wclist); e != list_end (wclist); e = list_next (e))
    {
        word_count_t *wc = list_entry (e, word_count_t, elem);
        len += wc->count;
          
    }

  return len;
}

word_count_t* find_word(word_count_list_t* wclist, char* word) {
  // idea: 

  struct list_elem *e;
  
  for (e = list_begin (wclist); e != list_end (wclist); e = list_next (e))
    {
        word_count_t *wc = list_entry (e, word_count_t, elem);
        if (strcmp(word, wc->word) == 0) {
            return wc;
        }
          
    }

    return NULL;
}

word_count_t* add_word(word_count_list_t* wclist, char* word) {
    word_count_t* w = find_word(wclist, word);

    if (w != NULL) {
      // increment count
        w->count++;
        return w;
    }

    // otherwise insert word and set the count to 1
    // w is null so allot space
    w = malloc(sizeof(word_count_t));
    w->count = 1;
    w->word = word;

    // list_push_back(struct list*, struct list_elem*);
    list_push_back(wclist, &w->elem);
    
    return w;

}

void fprint_words(word_count_list_t* wclist, FILE* outfile) {
  /* TODO */
  /* Please follow this format: fprintf(<file>, "%i\t%s\n", <count>, <word>); */

  struct list_elem* e;

  for (e = list_begin(wclist); e != list_end(wclist); e = list_next(e)) {
    word_count_t* w = list_entry(e, word_count_t, elem);
    fprintf(outfile, "%i\t%s\n", w->count, w->word);
  }

  // fprintf(outfile, "%i\t%s\n", w->count, w->word);
}

static bool less_list(const struct list_elem* ewc1, const struct list_elem* ewc2, void* aux) {
  
  word_count_t* w1 = list_entry(ewc1, word_count_t, elem);
  word_count_t* w2 = list_entry(ewc2, word_count_t, elem);

  // NOTE: aux is a function wrapper here, that is we need to cast the void* back to the bool function that it is
  // it compares the 2 words and returns a bool so its of the tyoe bool (*)(*w1, *w2) as defined below

  bool (*my_comparator)(const word_count_t*, const word_count_t*);
  my_comparator = (bool (*)(const word_count_t*, const word_count_t*)) aux;

  return (*my_comparator)(w1, w2);
}

void wordcount_sort(word_count_list_t* wclist,
                    bool less(const word_count_t*, const word_count_t*)) {
  list_sort(wclist, less_list, less);
}
