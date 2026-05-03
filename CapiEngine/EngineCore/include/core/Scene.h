#pragma once
#include <array>
#include <vector>
#include <functional>
#include <ec/ec.h>
#include <serialize/Serializable.h>
#include <string>
#include <memory>

namespace cme {
	class Camera;
	class GlobalLight;

	class Scene : public Serializable
	{
	private:
		/// @param gameObjectsByLayer vectores de objetos seg�n la layer a la que pertencen
		/// El vector de gameObjectsByLayer[n] tendr�a todos los objetos pertencientes a la layer n 
		std::array<std::vector<ec::entity_t>, ec::ent::maxGroupLayer> _gameObjectsByGroup;
		std::vector<ec::entity_t> _gizmos;

		Camera* _cam = nullptr;
		std::string _name = "Scene";

		GlobalLight* _globalLight = nullptr;
	public:
		Scene(std::string name);

		virtual ~Scene();

		/// @brief renderiza todos los objetos de la escena seg�n la layer y posteriormente renderiza la UI
		void render() const;

		/// @brief actualiza todos los objetos de la escena
		void update();

		/// @brief quita todos los objetos que no est�n activos de la escena
		void refresh();

		/// @brief Crea la cuadricula en el plano y = 0
		void createGrid();

		void addCubeToScene();

		/// @brief permite a�adir objetos a la escena
		/// @param grID Grupo al que a�adir al objeto creado
		std::shared_ptr<ec::Entity> addGameObject(Scene* scene, std::string name, ec::ent::groupID grID = ec::ent::None);
		ec::entity_t addGizmos();

		const std::array<std::vector<ec::entity_t>, ec::ent::maxGroupLayer>& getSceneObjects() { return _gameObjectsByGroup; }
		Camera* getCamera() { return _cam; }

		std::string name() { return _name; }

		void serialize(JsonSerializer& s) const override;
		void deserialize(JsonSerializer& s) override;

		GlobalLight* globalLight() { return _globalLight; }
	private:
		/// @brief inicializa la escena creando todos los objetos que vayan a haber en la misma
		virtual void init() {};
	};
}
