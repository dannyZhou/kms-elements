/*
 * (C) Copyright 2016 Kurento (http://kurento.org/)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <gst/gst.h>
#include "MediaPipeline.hpp"
#include <CompositeImplFactory.hpp>
#include "CompositeImpl.hpp"
#include <jsonrpc/JsonSerializer.hpp>
#include <KurentoException.hpp>
#include <gst/gst.h>
#include "kmscompositemixer.h"
#include "VideoLocation.hpp"

#define GST_CAT_DEFAULT kurento_composite_impl
GST_DEBUG_CATEGORY_STATIC (GST_CAT_DEFAULT);
#define GST_DEFAULT_NAME "KurentoCompositeImpl"

#define FACTORY_NAME "compositemixer"

namespace kurento
{

CompositeImpl::CompositeImpl (const boost::property_tree::ptree &conf,
                              std::shared_ptr<MediaPipeline> mediaPipeline) : HubImpl (conf,
                                    std::dynamic_pointer_cast<MediaObjectImpl> (mediaPipeline), FACTORY_NAME)
{
}

MediaObjectImpl *
CompositeImplFactory::createObject (const boost::property_tree::ptree &conf,
                                    std::shared_ptr<MediaPipeline> mediaPipeline) const
{
  return new CompositeImpl (conf, mediaPipeline);
}

void CompositeImpl::setVideosLocation (const
                                       std::vector<std::shared_ptr<VideoLocation>> &videosLocation)
{
  // TODO
  GArray *kmsLocations;
  kmsLocations = g_array_sized_new (FALSE, FALSE,
                                    sizeof (KmsCompositeVideoLocation), videosLocation.size() );

  for (const std::shared_ptr<VideoLocation> &location_ptr : videosLocation) {
    KmsCompositeVideoLocation kmsLocation;
    VideoLocation *location = location_ptr.get();

    kmsLocation.top = location->getTop();
    kmsLocation.left = location->getLeft();
    kmsLocation.height = location->getHeight();
    kmsLocation.width = location->getWidth();
    kmsLocation.handlerId = location->getHandlerId();

    g_array_append_val (kmsLocations, kmsLocation);
  }

  // check memory leak
  g_signal_emit_by_name (element, "set-videos-location", kmsLocations);
}

CompositeImpl::StaticConstructor CompositeImpl::staticConstructor;

CompositeImpl::StaticConstructor::StaticConstructor()
{
  GST_DEBUG_CATEGORY_INIT (GST_CAT_DEFAULT, GST_DEFAULT_NAME, 0,
                           GST_DEFAULT_NAME);
}

} /* kurento */
