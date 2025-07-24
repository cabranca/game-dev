#pragma once

#include <Cabrankengine/Core/Window.h>
struct GLFWwindow;

namespace cabrankengine {

    class LinuxWindow : public Window {
    public:
        LinuxWindow(const WindowProps& props);
        virtual ~LinuxWindow();

        void onUpdate() override;

        inline unsigned int getWidth() const override { return m_Data.Width; }
        inline unsigned int getHeight() const override { return m_Data.Height; }

        inline void setEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void setVSync(bool enabled) override;
        bool isVSync() const override;

    private:
        virtual void init(const WindowProps& props);
        virtual void shutdown();

        GLFWwindow* m_Window;

        struct WindowData {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

}
