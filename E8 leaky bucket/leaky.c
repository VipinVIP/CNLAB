#include <stdio.h>

int main() {
  int incoming, outgoing, buck_size, n, store = 0;
  printf("\nEnter bucket size, outgoing rate and no of ticks: ");
  scanf("%d %d %d", &buck_size, &outgoing, &n);

  while (n != 0) {
    printf("\nEnter the incoming packet size : ");
    scanf("%d", &incoming);

    if (incoming <= (buck_size - store)) {
      store += incoming;
      printf("Bucket buffer size %d out of %d\n", store, buck_size);
    } else {
      printf("Dropped %d no of packets\n", incoming - (buck_size - store));
      printf("Bucket buffer size %d out of %d\n", store, buck_size);
      store = buck_size;
    }
    if (outgoing > store) {
      store = 0;
      printf("Low flow After outgoing %d packets left out of %d in buffer\n",
             store, buck_size);
    } else {
      store = store - outgoing;
      printf("After outgoing %d packets left out of %d in buffer\n", store,
             buck_size);
    }

    n--;
  }
  while (store != 0) {
    if (store < outgoing) {
      store = 0;
    } else {
      store = store - outgoing;
    }

    printf("\nAfter outgoing %d packets left out of %d in buffer\n", store,
           buck_size);
  }
}
