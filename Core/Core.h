#pragma once

class Core final
{
    enum SideView : size_t
    {
        SV_Left = 0U,
        SV_Right,

        SV_Max
    };

    bool m_valid;
    bool m_quitRequest;

    sf::RenderWindow *m_window;
    sf::Event m_event;
    sf::Texture *m_texture;
    sf::Sprite *m_sprite;
    size_t m_sideView;

    Leap::Controller *m_leapController;
public:
    Core();
    ~Core();

    bool Initialize();
    void Terminate();

    bool DoPulse();
};
