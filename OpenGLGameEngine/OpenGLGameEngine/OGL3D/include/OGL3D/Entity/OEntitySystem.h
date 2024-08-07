#pragma once

#include <map>



class OEntity;
class OEntitySystem
{
public:
	OEntitySystem();
	~OEntitySystem();


private:
	std::map < size_t, std::map<OEntity*, std::unique_ptr<OEntity>>> m_entities;
};

