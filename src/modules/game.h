class Game {
public:
    unsigned int difficulty;
    bool solved;

    virtual void init() = 0;
    virtual void tick() = 0;
    virtual unsigned short int verify() = 0;
};
