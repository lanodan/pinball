/***************************************************************************
                          Light.h  -  description
                             -------------------
    begin                : Sun Jan 30 2000
    copyright            : (C) 2000 by 
    email                : 
 ***************************************************************************/

#ifndef LIGHT_H
#define LIGHT_H

#define EM_IGNORE_ANGLE_HALF 1
#define EM_IGNORE_ANGLE_FULL 2
#define EM_IGNORE_DISTANCE 4
#define EM_USE_BOUNDS 8

#include "Private.h"

#include "Node.h"
/**
 * <p>Light objects will light up its enviroment. As any other node subclass
 * it can be added to a group and moved around in the universe. Remember
 * that lights must also be added to the engine with the 'addLight'
 * function.</p>
 * <p>Example:<br><blockquote>
 * Group* groupL = new group;<br>
 * Light* l = new Light(1.0, 10.0);<br>
 * <br>
 * groupL->add(l);<br>
 * engine->add(groupL);<br>
 * <br>
 * engine->addLight(l);</blockquote>
 * </p>
 */
class Light {
 public:
	/** Light(constant, linear, quadratic, r, g, b). */
	Light(float c, float l, float q, float r=1.0, float g=1.0, float b=1.0);
	~Light();
	void setOn(bool on=true) { m_bOn = on;};
	bool getOn() { return m_bOn;};
	void setColor(float, float, float);
	void setProperty(int);
	void unsetProperty(int);
	void setBounds(float b) { m_fBounds = b; };
	void setIndex(int i);
	
	Vertex3D m_vtxSrc;
	Vertex3D m_vtxTrans;
	Vertex3D m_vtxAlign;
 private:
	friend class PointLightVisitor;
	friend class TransformVisitor;
	float m_fConstant;
	float m_fLinear;
	float m_fQuadratic;
	float m_fR, m_fG, m_fB;
	float m_fBounds;
	int m_iIndex;
	bool m_bOn;
	int m_iProperties;
};

#endif // LIGHT_H
