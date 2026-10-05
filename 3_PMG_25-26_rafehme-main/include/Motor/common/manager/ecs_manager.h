#ifndef __ECS_MANAGER_H__
#define __ECS_MANAGER_H__ 1


#include <vector>
#include <memory>
#include <unordered_map>
#include <optional>
#include <assert.h>
#include <algorithm>
#include <iterator>
#include <sol/sol.hpp>
#include <nlohmann/json.hpp>
#include "Motor/common/manager/mesh_manager.h"
#include "Motor/common/context/deserialize_context.h"

class ECSListBase {
public:
	virtual void grow() = 0;
	virtual size_t size() = 0;
	virtual void erase(size_t entity) = 0;
	virtual void BindLua(sol::state& lua, size_t entity) = 0;
	virtual void Serialize(nlohmann::json& json,size_t entity) = 0;
	virtual void Deserialize(const nlohmann::json& json, size_t entity, struct DeserializeContext& context) = 0;

};


template <typename T>
class ECSList : public ECSListBase {
public:

	ECSList(std::string name):
	name_(name)
	{

	}

	virtual void grow() { list_.emplace_back(); }
	virtual size_t size() { return list_.size(); }
	virtual void erase(size_t entity) { list_[entity].reset(); }
	virtual void BindLua(sol::state& lua, size_t entity) override{
		if (!list_[entity])
			return;

		if constexpr (requires(T & component)
		{
			component.BindLua(lua);
		}) {
			list_[entity]->BindLua(lua);
		}
	}

	virtual void Serialize(nlohmann::json& json,size_t entity)override {
		if (!list_[entity])
			return;

		if constexpr (requires(T & component)
		{
			component.Serialize(json);
		}) {
			list_[entity]->Serialize(json);
		}
	}

	virtual void Deserialize(const nlohmann::json& json, size_t entity, DeserializeContext& context)override {
		if (list_[entity])
			return;

		if (!json.contains(name_))
			return;

		list_[entity].emplace();

		if constexpr (requires(T & component){
			component.Deserialize(json, context);
		}) {
			list_[entity]->Deserialize(json, context);
		}
	}


	std::vector<std::optional<T>> list_;
	std::string name_;
};

class ECSManager {
public:

	size_t AddEntity() {
		size_t size = component.begin()->second->size();
		for (auto& c : component) {
			c.second->grow();
		}
		return size;
	}

	void RemoveEntity(size_t entity) {
		if (entity > component.begin()->second->size())return;

		for (auto& c : component) {
			c.second->erase(entity);
		}
	}
	size_t GetSize() const {
		return component.begin()->second->size();
	}

	void BindComponentsToLua(sol::state& lua,size_t e)const {
		for (auto& c : component) {
				c.second->BindLua(lua, e);
		}
	}

	void SerializeComponents(nlohmann::json& json, size_t e) const {
		for (auto& c : component) {
			c.second->Serialize(json,e);
		}
	}

	void DeserializeComponents(const nlohmann::json& json, size_t e, DeserializeContext& context) const {
		for (auto& c : component) {
			c.second->Deserialize(json, e, context);
		}
	}
	
	template<typename T>
	class Iterator {
	public:

		Iterator& operator++() {
			++list_;
			return *this;
		}

		T* operator*() {
			if (list_->has_value()) {
				return &list_->value();
			}

			return nullptr;
		}

		bool operator==(const Iterator& other) const {
			return list_ == other.list_;
		}
		bool operator!=(const Iterator& other) const {
			return list_ != other.list_;
		}

		std::vector<std::optional<T>>::iterator list_;
	};

	template<typename T>
	class Container {
	public:
		Iterator<T> begin() {
			return Iterator<T>{list_.begin()};
		}

		Iterator<T> end() {
			return Iterator<T>{list_.end()};
		}

		std::vector<std::optional<T>>& list_;
	};


	template<typename T>
	std::vector<std::optional<T>>& GetComponentVector();

	template<typename T>
	Container<T> GetContainer();

	template<typename T>
	void AddComponentType(std::string name);

	template<typename T>
	T* GetComponent(size_t entity);

	template<typename T>
	T* AddComponent(size_t entity);

	template<typename T>
	void RemoveComponent(size_t entity);

	template<typename T>
	bool HasComponent(size_t entity);

	

	std::unordered_map<std::size_t, std::unique_ptr<ECSListBase>> component;
};


template<typename T>
void ECSManager::AddComponentType(std::string name) {
	size_t hash = typeid(T).hash_code();
	using vt = std::unordered_map<std::size_t, std::unique_ptr<ECSListBase>>::value_type;
	std::unique_ptr<ECSListBase> ecslbptr = std::make_unique<ECSList<T>>(name);
	component.insert(vt{ hash,std::move(ecslbptr) });
}

template<typename T>
std::vector<std::optional<T>>& ECSManager::GetComponentVector()
{
	std::size_t hash = typeid(T).hash_code();
	auto it = component.find(hash);
	if (it == component.end()) {
		assert(false && "Component unknown");
	}

	auto derived = static_cast<ECSList<T>*>(it->second.get());
	assert(derived && "Bad cast: component type mismatch");

	return derived->list_;
}

template<typename T>
ECSManager::Container<T> ECSManager::GetContainer()
{
	std::vector<std::optional<T>>& vector = GetComponentVector<T>();
	return Container<T>{vector};
}

template<typename T>
T* ECSManager::GetComponent(size_t entity) {
	std::vector<std::optional<T>>& vector = GetComponentVector<T>();
	if (entity >= vector.size()) {
		return nullptr;
	}

	auto& listValue = vector[entity];
	if (listValue.has_value()) {
		return &listValue.value();
	}
	else {
		return nullptr;
	}
}

template<typename T>
T* ECSManager::AddComponent(size_t entity) {
	std::vector<std::optional<T>>& vector = GetComponentVector<T>();

	if (entity >= vector.size()) {
		return nullptr;
	}

	vector[entity].emplace();
	return &vector[entity].value();

}

template<typename T>
void ECSManager::RemoveComponent(size_t entity) {
	std::vector<std::optional<T>>& vector = GetComponentVector<T>();
	if (entity >= vector.size()) {
		vector.resize(entity + 1);
	}

	vector[entity].reset();
}

template<typename T>
bool ECSManager::HasComponent(size_t entity)
{
	std::vector<std::optional<T>>& vector = GetComponentVector<T>();
	if (!vector[entity])return false;

	return true;
}



#endif // !__ECS_MANAGER_H__
