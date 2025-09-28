#pragma once

#include <Cabrankengine/ECS/EntityManager.h>
#include <Cabrankengine/ECS/ComponentManager.hpp>

namespace cabrankengine {

    class Registry {
        public:
            Registry() {
                componentManager = std::make_unique<ComponentManager>();
                entityManager = std::make_unique<EntityManager>();
            }

            Entity createEntity() {
                return entityManager->createEntity();
            }

            void destroyEntity(Entity e) {
                entityManager->destroyEntity(e);
                componentManager->entityDestroyed(e);
            }

            template<typename T>
            void registerComponent() {
                componentManager->registerComponent<T>();
            }

            template<typename T>
            void addComponent(Entity e, T component) {
                componentManager->addComponent<T>(e, component);

                // Instead of setting the bit in the signature, copies, sets and copies again
                auto signature = entityManager->getSignature(e);
                signature.set(componentManager->getComponentType<T>(), true);
                entityManager->setSignature(e, signature);
            }

            template<typename T>
            void removeComponent(Entity e) {
                componentManager->removeComponent<T>(e);

                auto signature = entityManager->getSignature(e);
                signature.set(componentManager->getComponentType<T>(), false);
                entityManager->setSignature(e, signature);
            }

            template<typename T>
            T& getComponent(Entity e) {
                return componentManager->getComponent<T>(e);
            }

            Signature getSignature(Entity e) {
                return entityManager->getSignature(e);
            }

        private:
            std::unique_ptr<ComponentManager> componentManager;
            std::unique_ptr<EntityManager> entityManager;
    };
}
