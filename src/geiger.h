// geiger.h

#ifndef _GEIGER_H_
#define	_GEIGER_H_


#include "control.h"


class CD3DEngine;
enum ObjectType;



class CGeiger : public CControl
{
public:
	CGeiger(CInstanceManager* iMan);
	~CGeiger();

	BOOL	Create(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg);

	BOOL	EventProcess(const Event &event);

	void	Draw();

protected:
	float	DetectObject();

protected:
	ObjectType	m_type;		// objet � d�tecter
	float		m_radius;	// rayon de d�tection
	float		m_proxi;	// proximit� de l'objet (0..1)
	float		m_lastDetect;
	float		m_phase;
	BOOL		m_bLightOn;
};


#endif //_GEIGER_H_
