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


// Blueberry
#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>

// Qmitk
#include "QmitkMITKRegionGrowingView.h"

// Qt
#include <QMessageBox>

// MITK
#include "mitkImageAccessByItk.h"
#include "mitkITKImageImport.h"
#include "mitkProperties.h"
#include "mitkColorProperty.h"

// ITK
#include <itkConnectedThresholdImageFilter.h>
#include <itkCommand.h>

const std::string QmitkMITKRegionGrowingView::VIEW_ID = "org.mitk.views.regiongrowing";

void QmitkMITKRegionGrowingView::SetFocus()
{
  m_Controls.buttonPerformImageProcessing->setFocus();
}

void QmitkMITKRegionGrowingView::CreateQtPartControl( QWidget *parent )
{

 // create GUI widgets from the Qt Designer's .ui file
  m_Controls.setupUi( parent );
  
  // 创建一个QmitkPointListwidget并把它加入至.ui文件
 lstPoints = new QmitkPointListWidget();
  m_Controls.verticalLayout->addWidget(lstPoints);

  // 使用RenderWindowPart获取StdMultiWidget
  //QmitkStdMultiWidgetEditor* qSMWE = dynamic_cast<QmitkStdMultiWidgetEditor*>(GetRenderWindowPart());
  //m_MultiWidget = qSMWE->GetStdMultiWidget();
  m_MultiWidget = GetActiveStdMultiWidget();
  //让种子点集控件关联StdMultiWidget(用作十字线更新）
  lstPoints->SetMultiWidget(m_MultiWidget);

  //创建一个新的DataTreeNode使其包含带交互的Pointset
  m_PointSet = mitk::PointSet::New();

  //! Call back called when the user adds a point to the point set with the mouse.
  
  typedef itk::SimpleMemberCommand<QmitkMITKRegionGrowingView> MemberCommandType;
  typedef MemberCommandType::Pointer MemberCommandPointerType;
  MemberCommandPointerType dataChangedCommand;
  dataChangedCommand = MemberCommandType::New();
  dataChangedCommand->SetCallbackFunction( this, &QmitkMITKRegionGrowingView::UpdateLowerAndUpper );
  m_OnPointSetAddEventObserverTag = m_PointSet->AddObserver( mitk::PointSetAddEvent(), dataChangedCommand);
  std::cout << " add event" << std::endl;

  mitk::DataNode::Pointer pointSetNode = mitk::DataNode::New();
  pointSetNode->SetData(m_PointSet);
  pointSetNode->SetName("用作区域增长的种子点");
  pointSetNode->SetProperty("helper object", mitk::BoolProperty::New(true) );
  pointSetNode->SetProperty("layer", mitk::IntProperty::New(1024) );

  //把pointset加入至data tree（用作渲染并由其它模块访问)
  GetDataStorage()->Add( pointSetNode );

  //把GUI控件与点集关联
  lstPoints->SetPointSetNode(pointSetNode); 

  m_Controls.lowerEdit->setText(QString::number((int)(std::numeric_limits<int>::max()), 10));
  m_Controls.upperEdit->setText(QString::number((int)(std::numeric_limits<int>::min()), 10));

  connect( m_Controls.buttonPerformImageProcessing, SIGNAL(clicked()), this, SLOT(DoImageProcessing()) );
  // connect( lstPoints, SIGNAL(PointListChanged()), this, SLOT(UpdateLowerAndUpper()) );

  
  
}

QmitkMITKRegionGrowingView::~QmitkMITKRegionGrowingView( )
{
	if ( m_PointSet.IsNotNull() )
	{
		m_PointSet->RemoveObserver( m_OnPointSetAddEventObserverTag );
	}
}

void QmitkMITKRegionGrowingView::OnSelectionChanged( std::vector< mitk::DataNode * > nodes )
{ 
  // iterate all selected objects, adjust warning visibility
  foreach( mitk::DataNode::Pointer node, nodes )
  {
    if( node.IsNotNull() && dynamic_cast<mitk::Image*>(node->GetData()) )
    {
      m_Controls.labelWarning->setVisible( false );
      m_Controls.buttonPerformImageProcessing->setEnabled( true );
	  /*mitk::BaseData* data = node->GetData();
	  if (data)
	  {
		mitk::Image* image = dynamic_cast<mitk::Image*>( data );
		if (image)
		{
			m_Controls.lowerEdit->setText(QString::number((int)(image->GetScalarValueMin()), 10));
			m_Controls.upperEdit->setText(QString::number((int)(image->GetScalarValueMax()), 10));
		}
	  }*/
	  UpdateLowerAndUpper();
      return;
    }
  }

  m_Controls.labelWarning->setVisible( true );
  m_Controls.buttonPerformImageProcessing->setEnabled( false );
}


void QmitkMITKRegionGrowingView::DoImageProcessing()
{
	
  std::vector<mitk::DataNode*> nodes = this->GetDataManagerSelection();
  if (nodes.empty())  return;
	

  mitk::DataNode* node = nodes.front();

  if (!node)
  {
    // Nothing selected. Inform the user and return
    QMessageBox::information( NULL, "Template", "Please load and select an image before starting image processing.");
    return;
  }
	
  // here we have a valid mitk::DataNode

  // a node itself is not very useful, we need its data item (the image)
  mitk::BaseData* data = node->GetData();
  if (data)
  {
    // test if this data item is an image or not (could also be a surface or something totally different)
    mitk::Image* image = dynamic_cast<mitk::Image*>( data );
    if (image)
    {
      std::stringstream message;
      std::string name;
      message << "Performing image processing for image ";
      if (node->GetName(name))
      {
        // a property called "name" was found for this DataNode
        message << "'" << name << "'";
      }
      message << ".";
      MITK_INFO << message.str();

      // actually do something here...
	  // So we have an image. Let's see if the user has set some seed points already
		if ( m_PointSet->GetSize() == 0 )
		{
		  // no points there. Not good for region growing
		  QMessageBox::information( NULL, "Region growing functionality", 
										  "Please set some seed points inside the image first.\n"
										  "(hold Shift key and click left mouse button inside the image.)"
										);
		  return;
		}
		
		// actually perform region growing. Here we have both an image and some seed points
		AccessByItk_1( image, ItkImageProcessing, image->GetGeometry() ); // some magic to call the correctly templated function

    }
  }else{
	std::stringstream message;
    std::string name;
    message << "Performing image processing failed ";
	MITK_INFO << message.str();
  }
}

void QmitkMITKRegionGrowingView::UpdateLowerAndUpper()
{
  std::vector<mitk::DataNode*> nodes = this->GetDataManagerSelection();
  if (nodes.empty())  return;
  std::cout << "Update Lower and Upper" << std::endl;
   mitk::DataNode* node = nodes.front();

    if (!node)
  {
    // Nothing selected. Inform the user and return
    QMessageBox::information( NULL, "Template", "Please load and select an image before starting image processing.");
    return;
  }
	
  // here we have a valid mitk::DataNode

  // a node itself is not very useful, we need its data item (the image)
  mitk::BaseData* data = node->GetData();
  int min = std::numeric_limits<int>::max(), max=std::numeric_limits<int>::min();
  std::cout << "min: " << min << std::endl;
  std::cout << "max: " << max << std::endl;
  if (data)
  {
    // test if this data item is an image or not (could also be a surface or something totally different)
    mitk::Image* image = dynamic_cast<mitk::Image*>( data );
    if (image)
    {
	  mitk::Geometry3D* imageGeometry = image->GetGeometry();
	  mitk::PointSet::PointsContainer* points = m_PointSet->GetPointSet()->GetPoints();
	  for ( mitk::PointSet::PointsConstIterator pointsIterator = points->Begin(); 
			pointsIterator != points->End();
			++pointsIterator )
	  {
			// first test if this point is inside the image at all
			if ( !imageGeometry->IsInside( pointsIterator.Value()) ) 
			{
			  continue;
			}

			// "GetPixel" method is already available. 

			// double mitk::Image::GetPixelValueByIndex(const mitk::Index3D &position, unsigned int timestep)

			// double mitk::Image::GetPixelValueByWorldCoordinate(const mitk::Point3D& position, unsigned int timestep)

			int currentPixelValue = image->GetPixelValueByWorldCoordinate(pointsIterator.Value(), 0);
			std::cout << "currentPixelValue: " << currentPixelValue << std::endl;
			// adjust minimum and maximum values
			if (currentPixelValue > max)
			  max = currentPixelValue;

			if (currentPixelValue < min)
			  min = currentPixelValue;
			// convert world coordinates to image indices
			std::cout << "min: " << min << std::endl;
			std::cout << "max: " << max << std::endl;
			
	  }
	  if (m_PointSet->GetSize()>0)
	  {
		  min -= 30;
		  max += 30;
		 
		  m_Controls.lowerEdit->setText(QString::number((int)(min), 10));
		  m_Controls.upperEdit->setText(QString::number((int)(max), 10));
	  }else{
		  m_Controls.lowerEdit->setText(QString::number((int)(image->GetScalarValueMin()), 10));
		  m_Controls.upperEdit->setText(QString::number((int)(image->GetScalarValueMax()), 10));
	  }
	 
	}
  }
}

template < typename TPixel, unsigned int VImageDimension >
void QmitkMITKRegionGrowingView::ItkImageProcessing( itk::Image< TPixel, VImageDimension >* itkImage, mitk::Geometry3D* imageGeometry )
{
  typedef itk::Image< TPixel, VImageDimension > InputImageType;
  typedef typename InputImageType::IndexType    IndexType;
  
  // instantiate an ITK region growing filter, set its parameters
  typedef itk::ConnectedThresholdImageFilter<InputImageType, InputImageType> RegionGrowingFilterType;
  typename RegionGrowingFilterType::Pointer regionGrower = RegionGrowingFilterType::New();
  regionGrower->SetInput( itkImage ); // don't forget this

  // determine a thresholding interval
  IndexType seedIndex;
  TPixel min( std::numeric_limits<TPixel>::max() );
  TPixel max( std::numeric_limits<TPixel>::min() );
  mitk::PointSet::PointsContainer* points = m_PointSet->GetPointSet()->GetPoints();
  for ( mitk::PointSet::PointsConstIterator pointsIterator = points->Begin(); 
		pointsIterator != points->End();
		++pointsIterator ) 
  {
	// first test if this point is inside the image at all
	if ( !imageGeometry->IsInside( pointsIterator.Value()) ) 
	{
	  continue;
	}

	// convert world coordinates to image indices
	imageGeometry->WorldToIndex( pointsIterator.Value(), seedIndex);

	// get the pixel value at this point
	TPixel currentPixelValue = itkImage->GetPixel( seedIndex );

	// adjust minimum and maximum values
	if (currentPixelValue > max)
	  max = currentPixelValue;

	if (currentPixelValue < min)
	  min = currentPixelValue;

	regionGrower->AddSeed( seedIndex );
  }

  std::cout << "Values between " << min << " and " << max << std::endl;

  min -= 30;
  max += 30;

  if (m_Controls.lowerEdit->text().toInt()<m_Controls.upperEdit->text().toInt()) {
	 regionGrower->SetLower( m_Controls.lowerEdit->text().toInt() );
	 regionGrower->SetUpper( m_Controls.upperEdit->text().toInt() );
	 std::cout << "Use m_Controls" << std::endl;
  }else{
	  // set thresholds and execute filter
	  regionGrower->SetLower( min );
	  regionGrower->SetUpper( max );
  }

  regionGrower->Update();

  mitk::Image::Pointer resultImage = mitk::ImportItkImage( regionGrower->GetOutput() );
  mitk::DataNode::Pointer newNode = mitk::DataNode::New();
  newNode->SetData( resultImage );

  // set some properties
  newNode->SetProperty("binary", mitk::BoolProperty::New(true));
  newNode->SetProperty("name", mitk::StringProperty::New("dumb segmentation"));
  newNode->SetProperty("color", mitk::ColorProperty::New(1.0,0.0,0.0));
  newNode->SetProperty("volumerendering", mitk::BoolProperty::New(true));
  newNode->SetProperty("layer", mitk::IntProperty::New(1));
  newNode->SetProperty("opacity", mitk::FloatProperty::New(0.5));

  // add result to data tree
  this->GetDataStorage()->Add( newNode );
  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}
	
