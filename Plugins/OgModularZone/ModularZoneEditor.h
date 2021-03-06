///Modular Zone Plugin
///
/// Copyright (c) 2009 Gary Mclean
//
//This program is free software; you can redistribute it and/or modify it under
//the terms of the GNU Lesser General Public License as published by the Free Software
//Foundation; either version 2 of the License, or (at your option) any later
//version.
//
//This program is distributed in the hope that it will be useful, but WITHOUT
//ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
//FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
//
//You should have received a copy of the GNU Lesser General Public License along with
//this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//Place - Suite 330, Boston, MA 02111-1307, USA, or go to
//http://www.gnu.org/copyleft/lesser.txt.
////////////////////////////////////////////////////////////////////////////////*/

#pragma once

#include <vector>
#include "OgitorsPrerequisites.h"
#include "BaseEditor.h"
#include "OgitorsRoot.h"
#include "NodeEditor.h"
#include "ModularZonePlugin.h"

using namespace Ogitors;



namespace MZP
{
	class PortalEditor;
	struct ZoneInfo;
	
    class PluginExport ModularZoneEditor: public CNodeEditor
    {
        friend class ModularZoneFactory;
    public:
		virtual bool isNodeType() {return false;};//Prevent CNodeEditor descendants to behave as nodes
        /** @copydoc CBaseEditor::load() */
        virtual bool      load(bool async = true);
        /** @copydoc CBaseEditor::unLoad() */
        virtual bool      unLoad();
        /** @copydoc CBaseEditor::createProperties(OgitorsPropertyValueMap &) */
        virtual void      createProperties(OgitorsPropertyValueMap &params);
        /** @copydoc CBaseEditor::showBoundingBox(bool) */
        virtual void      showBoundingBox(bool bShow);
        /** @copydoc CBaseEditor::getAABB() */
        virtual Ogre::AxisAlignedBox getAABB() {if(mZoneMesh) return mZoneMesh->getBoundingBox();else return Ogre::AxisAlignedBox::BOX_NULL;};

		//virtual void     setSelectedImpl(bool bSelected); use this 
		virtual bool update(float timePassed);

		virtual bool getObjectContextMenu(UTFStringVector &menuitems);

		virtual void onObjectContextMenu(int menuresult);

		Ogre::SceneNode* getSceneNode(void){return mHandle;}
		void setPortalLocked(bool lock = true);
		bool getPortalLocked(void){return mPortalLocked;}
		void addPortal(PortalEditor*);//adds an existing portal
		Ogre::Entity* getZoneMesh(void){return mZoneMesh;}
		void setDesignMode(void);
		//inline bool getDesignMode(void){return mDesignMode;}
		bool mDragging;

    protected:
		void addPortal(void);//creates and adds a new portal (used in design mode only)
		void updateDesignInfo(void);//send any design changes to the zone selection widget
		void exportZone(void);// save a new zone design to a .zone file

		Ogre::Entity* mZoneMesh; //Enclosing mesh for the Zone
		std::vector<PortalEditor*> mPortals;  //container of portals attached to this zone

		bool mPortalLocked;//when portals a join set true
		ZoneInfo* mDesignInfo;//used when a new zone template is being designed
		

        /**
        * Constructor
        * @param factory Handle to marker editor factory
        */
        ModularZoneEditor(CBaseEditorFactory *factory);
        /**
        * Destructor
        */
        ~ModularZoneEditor() {};
  
		bool _loadZoneDescription(int);

		//PROPERTIES
		OgitorsProperty<int>* mZoneDescription; //id of the zone description file
		OgitorsProperty<int>* mPortalCount;//number of portals in this zone
		OgitorsProperty<Ogre::String>* mMeshName;//name of the mesh
		//SETTERS
		bool _setZoneTemplate(OgitorsPropertyBase* property, const int& value);
		bool _setPortalCount(OgitorsPropertyBase* property, const int& value);
		bool _setPortal(OgitorsPropertyBase* property, const Ogre::String& value);
		bool _setMeshName(OgitorsPropertyBase* property, const Ogre::String& value);
		void setSelectedImpl(bool bSelected);

    };


}

