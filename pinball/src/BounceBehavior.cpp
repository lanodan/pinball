/***************************************************************************
                          BouBeh.cpp  -  description
                             -------------------
    begin                : Thu Mar 9 2000
    copyright            : (C) 2000 by
    email                :
 ***************************************************************************/

#include <stdlib.h>
#include "BounceBehavior.h"
#include "Group.h"
#include "Pinball.h"
#include "Keyboard.h"

#define MAX_SPEED 0.5f
#define SPEED_FCT 0.8f
#define Y_GRAVITY -(SPEED_FCT*0.01) // -SPEED_FCT/100
#define Z_GRAVITY (SPEED_FCT*0.002) //  SPEED_FCT/500
#define BORDER 0.1f


#define CHECK_SPEED()    \
{                        \
	float l;               \
	if ( (l=EMath::vectorLength(m_vtxDir)) > MAX_SPEED ) { \
		l /= MAX_SPEED;      \
		m_vtxDir.x /= l;     \
		m_vtxDir.y /= l;     \
		m_vtxDir.z /= l;     \
	}	                     \
}


BounceBehavior::BounceBehavior(int ball) {
	m_iBall = ball;
	m_bAlive = false;
	m_bKnock = false;
	m_vtxDir.x = 0.0;
	m_vtxDir.y = 0.0;
	m_vtxDir.z = 0.0;
	m_vtxOldDir.x = 0.0;
	m_vtxOldDir.y = 0.0;
	m_vtxOldDir.z = 0.0;
	m_iDirFactor = 0;
	m_iCollisionPrio = 0;
	switch (m_iBall) {
		case PBL_BALL_1: if (p_Parent != NULL) p_Parent->setTranslation(-4,0,34); break;
		case PBL_BALL_2: if (p_Parent != NULL) p_Parent->setTranslation(-8, 0, 34); break;
		case PBL_BALL_3: if (p_Parent != NULL) p_Parent->setTranslation(-12, 0, 34); break;
		default: if (p_Parent != NULL) p_Parent->setTranslation(-16, 0, 34);
	}
}

BounceBehavior::~BounceBehavior() {
}

/* The current direction is need by behaviors objects of other balls.
 * The upcomming direction may change, therefore we need to give the old direction. */
void BounceBehavior::getDirection(Vertex3D & vtx) {
	vtx.x = m_vtxOldDir.x;
	vtx.y = m_vtxOldDir.y;
	vtx.z = m_vtxOldDir.z;
}

void BounceBehavior::StdOnSignal() {
	EM_COUT("BounceBehavior::onSignal()", 0);
	OnSignal( PBL_SIG_RESET_ALL) {
		m_bAlive = false;
		switch (m_iBall) {
			case PBL_BALL_1: p_Parent->setTranslation(-4,0,34); break;
			case PBL_BALL_2: p_Parent->setTranslation(-8, 0, 34); break;
			case PBL_BALL_3: p_Parent->setTranslation(-12, 0, 34); break;
			default: p_Parent->setTranslation(-16, 0, 34);
		}
	}
	#define ACTIVATE_BALL									\
		m_bAlive = true;										\
		p_Parent->setTranslation(m_iBall, 0, 20);	\
		m_vtxDir.x = -0.2f*SPEED_FCT; 	\
		m_vtxDir.y = 0;									\
		m_vtxDir.z = -0.88*SPEED_FCT + 0.02*SPEED_FCT*rand()/RAND_MAX;

	// debug stuff
	if (Keyboard::isKeyDown(SDLK_a)) {
		ACTIVATE_BALL
	}

	OnSignal( PBL_SIG_ACTIVATE_BALL_1 ) {
		if (m_iBall == PBL_BALL_1) {
			EM_COUT("BounceBehavior::onSignal() ball 1", 0);
			ACTIVATE_BALL
		}
	}
	OnSignal( PBL_SIG_ACTIVATE_BALL_2 ) {
		if (m_iBall == PBL_BALL_2) {
			ACTIVATE_BALL
		}
	}
	OnSignal( PBL_SIG_ACTIVATE_BALL_3 ) {
		if (m_iBall == PBL_BALL_3) {
			ACTIVATE_BALL
		}
	}
	OnSignal( PBL_SIG_ACTIVATE_BALL_4 ) {
		if (m_iBall == PBL_BALL_4) {
			ACTIVATE_BALL
		}
	}
	#undef ACTIVATE_BALL
}

void BounceBehavior::onTick() {
	EmAssert(p_Parent != NULL, "BounceBehavior::onTick()");
	if (!m_bAlive) return;

	// reset collision
	m_iCollisionPrio = 0;

	// debug stuff
	if (Keyboard::isKeyDown(KEY_I)) m_vtxDir.z -= 0.01f;
	if (Keyboard::isKeyDown(KEY_K)) m_vtxDir.z += 0.01f;
	if (Keyboard::isKeyDown(KEY_J)) m_vtxDir.x -= 0.01f;
	if (Keyboard::isKeyDown(KEY_L)) m_vtxDir.x += 0.01f;

	// TODO: better table bump
	if (Keyboard::isKeyDown(KEY_SPACE)) {
		if (!m_bKnock) {
			m_bKnock = true;
			m_vtxDir.z -= SPEED_FCT*0.2f;
		}
	} else {
		m_bKnock = false;
	}

	// Gravity
	m_vtxDir.z += Z_GRAVITY;
	float x, y, z;
	p_Parent->getTranslation(x, y, z);
	if (y <= 0) {
		m_vtxDir.y = 0;
		p_Parent->setTranslation(x, 0, z);
	} else {
		m_vtxDir.y += Y_GRAVITY;
	}
	m_vtxOldDir = m_vtxDir;

	// move the ball
	p_Parent->addTranslation(m_vtxDir.x, m_vtxDir.y, m_vtxDir.z);

	if (z > 33) {
		m_bAlive = false;
		switch (m_iBall) {
			case PBL_BALL_1: SendSignal( PBL_SIG_BALL_1_DEAD ); break;
			case PBL_BALL_2: SendSignal( PBL_SIG_BALL_2_DEAD ); break;
			case PBL_BALL_3: SendSignal( PBL_SIG_BALL_3_DEAD ); break;
			default: SendSignal( PBL_SIG_BALL_4_DEAD );
		}
	}
}

// each collision changes the direction of the ball
void BounceBehavior::onCollision(const Vertex3D & vtxW, const Vertex3D & vtxOwn, Group * pGroup) {
	EmAssert(p_Parent != NULL && pGroup != NULL, "BounceBehavior::onCollision()");
	if (!m_bAlive) return;
	EM_COUT("BounceBehavior::onCollision() wall " << vtxOwn.x <<" "<< vtxOwn.y <<" "<< vtxOwn.z, 0);

	// Undo last translation
	p_Parent->addTranslation(-m_vtxDir.x, -m_vtxDir.y, -m_vtxDir.z);

	Vertex3D vtxWall, vtxNew;
	vtxWall.x = -vtxOwn.x;
	vtxWall.y = -vtxOwn.y;
	vtxWall.z = -vtxOwn.z;

	// change direction depending on which type the colliding object is
	if (pGroup->getUserProperties() & PBL_BUMPER) {
		if (m_iCollisionPrio > 3) return;
		m_iCollisionPrio = 3;
		EMath::reflectionDamp(m_vtxOldDir, vtxWall, m_vtxDir, (float)1.0, (float)SPEED_FCT*0.5, 1, true);
		EM_COUT("BounceBehavior.onCollision() bumper\n", 0);
	} else if (pGroup->getUserProperties() & PBL_ACTIVE_ARM) {
		if (m_iCollisionPrio > 3) return;
		m_iCollisionPrio = 3;
		EMath::reflectionDamp(m_vtxOldDir, vtxWall, m_vtxDir, (float)1.0, (float)SPEED_FCT, 1, true);
		EM_COUT("BounceBehavior.onCollision() active arm\n", 0);
	} else if (pGroup->getUserProperties() & PBL_TRAP_BOUNCE) {
		if (m_iCollisionPrio > 3) return;
		m_iCollisionPrio = 3;
		// Bbounce ball out of cave, 0 means ignore old speed
		EMath::reflectionDamp(m_vtxOldDir, vtxWall, m_vtxDir, (float)0, (float)SPEED_FCT, 1, true);
		EM_COUT("BounceBehavior.onCollision() cave bounce\n", 0);
	} else if (pGroup->getUserProperties() & PBL_TRAP) {
		if (m_iCollisionPrio > 2) return;
		m_iCollisionPrio = 2;
		float tx, ty, tz;
		float bx, by, bz;
		// move the ball slowly towards the center of the cave
		pGroup->getTranslation(tx, ty, tz);
		p_Parent->getTranslation(bx, by, bz);
		m_vtxDir.x = (tx-bx)*0.1;
		m_vtxDir.y = 0;
		m_vtxDir.z = (tz-bz)*0.1;
		EM_COUT("BounceBehavior.onCollision() cave\n", 0);
	} else if (pGroup->getUserProperties() & PBL_UNACTIVE_ARM) {
		if (m_iCollisionPrio > 1) return;
		m_iCollisionPrio = 1;
		EMath::reflectionDamp(m_vtxOldDir, vtxWall, m_vtxDir, (float)0.2, 0, 1);
		EM_COUT("BounceBehavior.onCollision() unactive arm\n", 0);
	} else if (pGroup->getUserProperties() & PBL_WALLS) {
		if (m_iCollisionPrio > 1) return;
		m_iCollisionPrio = 1;
		EMath::reflectionDamp(m_vtxOldDir, vtxWall, m_vtxDir, (float)0.5, 0, 1);
		EM_COUT("BounceBehavior.onCollision() walls\n" , 0);
	} else if (pGroup->getUserProperties() & (PBL_BALL_1 | PBL_BALL_2 | PBL_BALL_3 | PBL_BALL_4)) {
		if (m_iCollisionPrio > 0) return;
		m_iCollisionPrio = 0;
		BounceBehavior* beh = (BounceBehavior*) pGroup->getBehavior(0);
		if (beh != NULL) {
			Vertex3D vtxDir2, vtxPrj1, vtxPrj2;
			beh->getDirection(vtxDir2);
			// the speed given from the other ball to this ball is the projection of the speed
			// onto the wall
			EMath::projection(vtxDir2, vtxWall, vtxPrj2);
			// the speed given from this ball to the other ball is the projection of the speed
			// onto the inverse wall (which was vtxOwn).
			EMath::projection(m_vtxOldDir, vtxOwn, vtxPrj1);
			// From my physics book m1*u1 + m2u2 = m1*v1 + m2*v2. (m1 == m2 in this case)
			// Gives us v1 = u1 + vtxPrj2 - vtxPrj1. And v2 = u2 + vtxPrj1 - vtxPrj2.
			m_vtxDir.x += (vtxPrj2.x - vtxPrj1.x);
			m_vtxDir.y += (vtxPrj2.y - vtxPrj1.y);
			m_vtxDir.z += (vtxPrj2.z - vtxPrj1.z);
			/*
			// I have no idea of how I did this one (yes I did it myself). 
			// It was a trail and error thing, but it works quiet (spelling?) well.
			// I don't think it's 100% physically correct though.
			Vertex3D vtxDir2;
			beh->getDirection(vtxDir2);
			EMath::projection(vtxDir2, vtxWall, vtxDir2);
			EMath::reflectionDamp(m_vtxDir, vtxWall, m_vtxDir, (float)1, 0, (float)0.5);
			m_vtxDir.x += vtxDir2.x*0.5;
			m_vtxDir.y += vtxDir2.y*0.5;
			m_vtxDir.z += vtxDir2.z*0.5;
			*/
		}
		EM_COUT("BounceBehavior.onCollision() ball\n", 0);
	} else {
		EM_COUT("BounceBehavior.onCollision() unknown\n" ,1);
	}
	
	CHECK_SPEED()

	p_Parent->addTranslation(m_vtxDir.x, m_vtxDir.y, m_vtxDir.z);
}

