/*

Copyright © 2019 University of California, Berkeley

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

word_count provides lists of words and associated count

Functional methods take the head of a list as first arg.
Mutators take a reference to a list as first arg.
*/

#include "word_count.h"

/* Basic utilities */

char *new_string(char *str) {
  char *new_str = (char *) malloc(strlen(str) + 1);
  if (new_str == NULL) {
    return NULL;
  }
  return strcpy(new_str, str);
}

int init_words(WordCount **wclist) { // why double pointer? because its a pointer to the head of the lsit which itself is a pointer
  /* Initialize word count.
     Returns 0 if no errors are encountered
     in the body of this function; 1 otherwise.
  */
  *wclist = NULL;
  return 0;
}

ssize_t len_words(WordCount *wchead) {
  /* Return -1 if any errors are
     encountered in the body of
     this function.
  */
    size_t len = 0;
  WordCount *wc = wchead;
  if (wc == NULL) {
    return -1;
  }
  while (wc != NULL) {
    len += 1;
    wc = wc->next;
  }

  return len;
}

WordCount *find_word(WordCount *wchead, char *word) {
  /* Return count for word, if it exists */
  WordCount *wc = wchead;
  
  while (wc) {
    if (strcmp(wc->word, word) == 0) {
      return wc;
    }
    wc = wc->next;
  }

  return wc;
}

int add_word(WordCount **wclist, char *word) {
  /* If word is present in word_counts list, increment the count.
     Otherwise insert with count 1.
     Returns 0 if no errors are encountered in the body of this function; 1 otherwise.
  */
  WordCount *wc = *wclist; // pointing to the head node

  // what if wc == NULL? create a new head
  if (wc == NULL) {
    wc = malloc(sizeof(WordCount));

    if (wc == NULL) {
      return 1;
    }

    wc->count = 1;
    wc->word = new_string(word);
    wc->next = NULL;

    *wclist = wc;

    return 0;
  }

  // else wc is not null and we must find the word
  wc = find_word(*wclist, word);
  if (wc != NULL) {
    wc->count += 1;

    return 0;
  }

  // if we couldnt find the word, then insert it with count 1
  // so now wc is null as no word found, so go to the end of list and add a node
     wc = *wclist;
  while (wc->next != NULL) {
    wc = wc->next;
  }

  wc->next = malloc(sizeof(WordCount));
  if (wc->next == NULL) {
    return 1;
  }
  wc->next->count = 1;
  wc->next->word = new_string(word);
  wc->next->next = NULL;

 return 0;
}

void fprint_words(WordCount *wchead, FILE *ofile) {
  /* print word counts to a file */
  WordCount *wc;
  for (wc = wchead; wc; wc = wc->next) {
    fprintf(ofile, "%i\t%s\n", wc->count, wc->word);
  }
}
