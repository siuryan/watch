#include "Health.h"
#include "Arduino.h"

Health::Health() : footsteps(0), x_agg{}, last_change(0) {}

void push_front(double *arr, double val, int n) {
    for (int i = 0; i < n-1; ++i) {
        arr[i+1] = arr[i];
    }
    arr[0] = val;
}

double average(double *arr, int n) {
    double sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += arr[i];
    }
    return sum/n;
}

void Health::update_footsteps(const double *position) {
    double avg1 = average(x_agg, 5);
    push_front(x_agg, position[0], 5);
    double avg2 = average(x_agg, 5);
    double change = avg1 - avg2;

    if (((change < 0 && last_change > 0) ||
         (change > 0 && last_change < 0)) &&
        abs(avg1-avg2) > SPIKE_THRESHOLD) {
        footsteps++;
    }

    last_change = change;
}

int Health::get_footsteps() {
    return footsteps;
}
