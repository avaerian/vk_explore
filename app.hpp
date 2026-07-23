#ifndef _APP_HPP
#define _APP_HPP

#include <cstdlib>
#include <stdexcept>
#include <cstring>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifndef NDEBUG // is debug
#define ENABLE_VALIDATION_LAYERS
#endif

class App {
    public:
        void run() {
            initWindow();
            initVk();
            mainLoop();
            cleanup();
        }

    private:
        GLFWwindow* wnd;
        VkInstance vk;

        void initWindow() {
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE); // test

            if(!glfwVulkanSupported())
                throw std::runtime_error("Vulkan not supported by GLFW");

            wnd = glfwCreateWindow(800, 600, "Lumbern", nullptr, nullptr);
        }

        void createInstance() {
#ifdef ENABLE_VALIDATION_LAYERS
            printf("Hello, world!\n");
            if(!checkValidationLayerSupport())
                throw std::runtime_error("Requested validation layers not available");
#endif
            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "Lumbern";
            appInfo.pEngineName = "Mojave";
            appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
            appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
            appInfo.apiVersion = VK_API_VERSION_1_3;
            
            VkInstanceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

#ifdef ENABLE_VALIDATION_LAYERS
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
#else
            createInfo.enabledLayerCount = 0;
#endif

            std::vector<const char*> exts = getRequiredExtensions();
            createInfo.enabledExtensionCount = static_cast<uint32_t>(exts.size());
            createInfo.ppEnabledExtensionNames = exts.data();

            if(vkCreateInstance(&createInfo, nullptr, &vk) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create Vulkan instance");
            } 
        }

        bool checkValidationLayerSupport() {
            uint32_t layersLen;
            vkEnumerateInstanceLayerProperties(&layersLen, nullptr);

            VkLayerProperties layers[layersLen];
            vkEnumerateInstanceLayerProperties(&layersLen, layers);
            
            for(auto requestedLayer : validationLayers) {
                bool found = false;
                for(auto providedLayer : layers) {
                    if(strcmp(requestedLayer, providedLayer.layerName)) {
                        found = true;
                        break;
                    }
                }
                if(!found)
                    return false;
            }

            return true;
        }

        std::vector<const char*> getRequiredExtensions() {
            uint32_t extsCount = 0;
            const char** exts = glfwGetRequiredInstanceExtensions(&extsCount);

            std::vector<const char*> res(exts, exts + extsCount);

#ifdef ENABLE_VALIDATION_LAYERS
            res.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
            return res;
        }

        void initVk() {
            createInstance();
            //glfwCreateWindowSurface
        }

        void mainLoop() {
            while(!glfwWindowShouldClose(wnd)) {
                glfwPollEvents();
            }
        }

        void cleanup() {
            vkDestroyInstance(vk, nullptr);

            // destroy window
            glfwDestroyWindow(wnd);
            glfwTerminate();
        }
};


#endif // _APP_HPP
