/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision$ 
 
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef org_gdmc_segmentation_regiongrowing_Activator_h
#define org_gdmc_segmentation_regiongrowing_Activator_h

#include <ctkPluginActivator.h>

namespace mitk {

class org_gdmc_segmentation_regiongrowing_Activator :
  public QObject, public ctkPluginActivator
{
  Q_OBJECT
  Q_INTERFACES(ctkPluginActivator)

public:

  void start(ctkPluginContext* context);
  void stop(ctkPluginContext* context);

}; // org_gdmc_segmentation_regiongrowing_Activator

}

#endif // org_gdmc_segmentation_regiongrowing_Activator_h
