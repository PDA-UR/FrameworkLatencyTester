#ifndef PARPORT_H
#define PARPORT_H

class Parport : GPIO {
    private:
        thread read_thread;
        int fd;
        void trigger_click(void);
        void trigger_bright(void);
        void trigger_bright_2(void);

    public:
        Parport();
        void read();
}

#endif
