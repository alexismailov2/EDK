/*******************************************************************************
 Copyright (C) 2018 Philips Lighting Holding B.V.
 All Rights Reserved.
 ********************************************************************************/

#include <huestream/effect/animation/animations/FramesAnimation.h>
#include <huestream/common/util/HueMath.h>

#include <iostream>
#include <string>
#include <memory>

namespace huestream {

    std::string FramesAnimation::AttributeFps = "Fps";
    double FramesAnimation::GetFps() const { return _fpms * 1000.0; }
    void FramesAnimation::SetFps(const double &fps) { _fpms = fps / 1000.0; _frameLength = 1000.0 / fps; }

    PROP_IMPL(FramesAnimation, std::shared_ptr<std::vector<uint16_t>>, frames, Frames);

    FramesAnimation::FramesAnimation() : FramesAnimation(24) {}

    FramesAnimation::FramesAnimation(double fps)
            : RepeatableAnimation(0), _frames(std::make_shared<std::vector<uint16_t>>()) {
        SetFps(fps);
    }

    FramesAnimation::FramesAnimation(double fps, unsigned int size) : FramesAnimation(fps) {
        _frames->reserve(size);
    }
            
    AnimationPtr FramesAnimation::Clone() {
        return std::make_shared<FramesAnimation>(*this);
    }

    void FramesAnimation::UpdateValue(double *value, double positionMs) {
        if (_frames->size() == 0)
            return;

        auto fracIndex = positionMs * _fpms;
        auto roundIndex = std::floor(fracIndex);
        auto index = static_cast<std::vector<uint16_t>::size_type>(roundIndex);

        if (index >= _frames->size() - 1) {
            *value = static_cast<double>(_frames->back()) / UINT16_MAX;
        } else {
            auto position = (fracIndex - roundIndex) * _frameLength;
            auto start = static_cast<double>((*_frames)[index]) / UINT16_MAX;
            auto end = static_cast<double>((*_frames)[index + 1]) / UINT16_MAX;
            *value = HueMath::linearTween(position, start, end, _frameLength);
        }
    }

    double FramesAnimation::GetLengthMs() const {
        return _frames->size() * _frameLength;
    }

    void FramesAnimation::Append(const double frame) {
        _frames->push_back(static_cast<uint16_t>(frame * UINT16_MAX));
    }

    void FramesAnimation::Serialize(JSONNode *node) const {
        RepeatableAnimation::Serialize(node);

        SerializeValue(node, AttributeFps, _fpms * 1000.0);

        JSONNode arrayNode(JSON_ARRAY);
        for (auto &f : *_frames) {
            JSONNode v;
            v.push_back(JSONNode("", f / UINT16_MAX));
            arrayNode.push_back(v);
        }
        arrayNode.set_name(AttributeFrames);
        node->push_back(arrayNode);
    }

    void FramesAnimation::Deserialize(JSONNode *node) {
        RepeatableAnimation::Deserialize(node);

        if (SerializerHelper::IsAttributeSet(node, AttributeFps)) {
            auto jsonFps = (*node)[AttributeFps];
            SetFps(jsonFps.as_float());
        }

        if (SerializerHelper::IsAttributeSet(node, AttributeFrames)) {
            auto jsonFrames = (*node)[AttributeFrames];

            _frames->clear();
            _frames->reserve(jsonFrames.size());

            for (auto &jsonFrame : jsonFrames) {
                Append(jsonFrame.as_float());
            }
        }
    }

    std::string FramesAnimation::GetTypeName() const {
        return type;
    }

}  // namespace huestream
