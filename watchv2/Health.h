class Health {

    public:
    static const double SPIKE_THRESHOLD = .75;

    Health();
    void update_footsteps(const double *position);
    int get_footsteps();

    private:
    int footsteps;
    double x_agg[5];

    double last_change;
};
