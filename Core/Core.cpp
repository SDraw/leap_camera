#include "stdafx.h"
#include "Core/Core.h"

SFML_DEFINE_DISCRETE_GPU_PREFERENCE

const sf::Vector2f g_spriteScale(1.f, 2.f);

Core::Core()
{
    m_valid = false;
    m_quitRequest = false;

    m_window = nullptr;

    m_texture = nullptr;
    m_sprite = nullptr;
    m_sideView = SV_Left;

    m_leapController = nullptr;
}
Core::~Core()
{
}

bool Core::Initialize()
{
    if(!m_valid)
    {
        sf::VideoMode l_mode(640U, 480U, 32U);
        sf::ContextSettings l_context(0U, 0U, 0U, 3U, 0U, 0U, false);
        m_window = new sf::RenderWindow(l_mode, "Leap Motion Camera Viewer", sf::Style::Titlebar | sf::Style::Close, l_context);
        m_window->setFramerateLimit(60U);
        m_window->setVerticalSyncEnabled(false);
        m_window->setKeyRepeatEnabled(false);

        if(m_window->setActive(true))
        {
            m_texture = new sf::Texture();
            m_sprite = new sf::Sprite();
            m_sprite->setScale(g_spriteScale);

            m_leapController = new Leap::Controller();
            m_leapController->setPolicy(Leap::Controller::PolicyFlag::POLICY_IMAGES);

            m_valid = true;
        }
    }
    return m_valid;
}

void Core::Terminate()
{
    delete m_leapController;
    m_leapController = nullptr;

    delete m_sprite;
    m_sprite = nullptr;

    delete m_texture;
    m_texture = nullptr;

    if(m_window) m_window->close();
    delete m_window;
    m_window = nullptr;

    m_valid = false;
}

bool Core::DoPulse()
{
    m_window->clear();

    while(m_window->pollEvent(m_event))
    {
        switch(m_event.type)
        {
            case sf::Event::Closed:
                m_quitRequest = true;
                break;
            case sf::Event::KeyPressed:
            {
                if(m_event.key.code == sf::Keyboard::K)
                {
                    m_sideView += 1U;
                    m_sideView %= SV_Max;
                }
            } break;
        }
    }

    if(m_leapController->isConnected())
    {
        const Leap::ImageList l_images = m_leapController->images();
        const size_t l_imagesCount = std::max(static_cast<size_t>(l_images.count()), static_cast<size_t>(SV_Max));
        if(l_imagesCount > 0U)
        {
            for(size_t i = 0U; i < l_imagesCount; i++)
            {
                if(i == m_sideView)
                {
                    const Leap::Image &l_image = l_images[i];
                    const size_t l_imageWidth = static_cast<size_t>(l_image.width());
                    const size_t l_imageHeight = static_cast<size_t>(l_image.height());

                    if((l_imageWidth > 0U) && (l_imageHeight > 0U))
                    {

                        // First init
                        if(m_texture->getNativeHandle() == 0)
                        {
                            m_texture->create(l_imageWidth, l_imageHeight);
                            m_sprite->setTexture(*m_texture, true);
                        }

                        // Check sizes
                        const sf::Vector2u l_textureSize = m_texture->getSize();
                        if((l_textureSize.x != l_imageWidth) || (l_textureSize.y != l_imageHeight))
                        {
                            delete m_texture;
                            m_texture = new sf::Texture();
                            m_texture->create(l_imageWidth, l_imageHeight);
                            m_sprite->setTexture(*m_texture, true);
                        }

                        // Update
                        const unsigned char *l_imageData = l_image.data();
                        std::vector<unsigned char> l_textureData(l_imageWidth*l_imageHeight * 4U, '\0');
                        size_t l_cursor = 0U;
                        for(size_t j = 0U; j < l_imageWidth; j++)
                        {
                            for(size_t k = 0U; k < l_imageHeight; k++)
                            {
                                const size_t l_dataCursor = l_cursor * 4U;
                                const unsigned char l_brightness = l_imageData[l_cursor];
                                l_textureData[l_dataCursor] = l_brightness;
                                l_textureData[l_dataCursor + 1U] = l_brightness;
                                l_textureData[l_dataCursor + 2U] = l_brightness;
                                l_textureData[l_dataCursor + 3U] = 255U;

                                l_cursor++;
                            }
                        }
                        m_texture->update(l_textureData.data());
                    }

                    break;
                }
            }
        }
    }

    m_window->draw(*m_sprite);

    m_window->display();

    return !m_quitRequest;
}
