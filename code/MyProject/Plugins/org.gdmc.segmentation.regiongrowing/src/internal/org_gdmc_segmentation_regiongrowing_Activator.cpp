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


#include "org_gdmc_segmentation_regiongrowing_Activator.h"

#include <QtPlugin>

#include "QmitkMITKRegionGrowingView.h"

namespace mitk {

void org_gdmc_segmentation_regiongrowing_Activator::start(ctkPluginContext* context)
{
  BERRY_REGISTER_EXTENSION_CLASS(QmitkMITKRegionGrowingView, context)
}

void org_gdmc_segmentation_regiongrowing_Activator::stop(ctkPluginContext* context)
{
  Q_UNUSED(context)
}

}

Q_EXPORT_PLUGIN2(org_gdmc_segmentation_regiongrowing, mitk::org_gdmc_segmentation_regiongrowing_Activator)
