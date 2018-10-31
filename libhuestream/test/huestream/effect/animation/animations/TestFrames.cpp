#include <huestream/effect/animation/animations/FramesAnimation.h>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test/huestream/common/TestSerializeBase.h"

using testing::ElementsAre;

namespace huestream {

class TestFrames : public testing::Test, public TestSerializeBase {
    double _timeMs;
 protected:
    static constexpr double precision = 1.0 / UINT16_MAX;

    virtual double AddMillisecondsAndReturnValue(FramesAnimation *frames, double positionMs) {
        _timeMs += positionMs;
        frames->SetMarker(_timeMs);
        return frames->GetValue();
    }

    void SetUp() override {
        _timeMs = 0;
    }

    void TearDown() override {
    }
};

TEST_F(TestFrames, Length) {
    auto framesAnimation = FramesAnimation(100);
    auto frames = std::make_shared<std::vector<uint16_t>>();
    frames->push_back(4);
    frames->push_back(5);
    frames->push_back(6);
    framesAnimation.SetFrames(frames);
    EXPECT_DOUBLE_EQ(framesAnimation.GetLengthMs(), 30);
}

TEST_F(TestFrames, Interpolation) {
    auto f = FramesAnimation(100);
    f.Append(0.1);
    f.Append(0.2);
    f.Append(0.3);
    f.Append(0.5);
    f.Append(0.9);
    f.Append(0.3);

    EXPECT_EQ(f.GetFrames()->size(), 6);
    EXPECT_NEAR(AddMillisecondsAndReturnValue(&f, 0), 0.1, precision);
    EXPECT_NEAR(AddMillisecondsAndReturnValue(&f, 5), 0.15, precision);
    EXPECT_NEAR(AddMillisecondsAndReturnValue(&f, 5), 0.2, precision);
    EXPECT_NEAR(AddMillisecondsAndReturnValue(&f, 2), 0.22, precision);
    EXPECT_NEAR(AddMillisecondsAndReturnValue(&f, 24), 0.74, precision);
    EXPECT_NEAR(AddMillisecondsAndReturnValue(&f, 4), 0.9, precision);
    EXPECT_NEAR(AddMillisecondsAndReturnValue(&f, 5), 0.6, precision);
}

TEST_F(TestFrames, AfterLastFrame) {
    auto f = FramesAnimation(10, 3);
    f.Append(0.3);
    f.Append(0.2);
    f.Append(0.1);
    EXPECT_NEAR(AddMillisecondsAndReturnValue(&f, 250), 0.1, precision);
    EXPECT_NEAR(AddMillisecondsAndReturnValue(&f, 100), 0.1, precision);
}

TEST_F(TestFrames, Empty) {
    auto f = FramesAnimation(10);
    EXPECT_NEAR(AddMillisecondsAndReturnValue(&f, 100), 0, precision);
}

TEST_F(TestFrames, Serialize) {
    auto f = FramesAnimation(10);
    f.Append(0.5);
    f.Append(0.7);
    
    JSONNode node;
    f.Serialize(&node);
    
    ASSERT_AttributeIsSetAndStringValue(node, Serializable::AttributeType, FramesAnimation::type);
    ASSERT_AttributeIsSetAndDoubleValue(node, FramesAnimation::AttributeFps, 10);
    ASSERT_TRUE(SerializerHelper::IsAttributeSet(&node, FramesAnimation::AttributeFrames));
    ASSERT_EQ(node[FramesAnimation::AttributeFrames].type(), JSON_ARRAY);
    ASSERT_EQ(node[FramesAnimation::AttributeFrames].as_array().size(), 2);
}

TEST_F(TestFrames, Deserialize) {
    JSONNode node;
    AddStringAttribute(&node, Serializable::AttributeType, FramesAnimation::type);
    AddDoubleAttribute(&node, FramesAnimation::AttributeFps, 33.33);

    JSONNode framesNode(JSON_ARRAY);
    framesNode.set_name(FramesAnimation::AttributeFrames);
    framesNode.push_back(JSONNode("", 0.9));
    framesNode.push_back(JSONNode("", 0.2));
    framesNode.push_back(JSONNode("", 0.9));
    node.push_back(framesNode);

    auto f = FramesAnimation();
    f.Deserialize(&node);

    ASSERT_DOUBLE_EQ(f.GetFps(), 33.33);
    auto frames = f.GetFrames();
    ASSERT_EQ(3, frames->size());
}

TEST_F(TestFrames, Clone) {
    auto f = FramesAnimation(10);
    f.Append(0.5);
    auto f_clone = f.Clone();
    ASSERT_NE(nullptr,            f_clone);
    EXPECT_EQ(f.GetValue(),       f_clone->GetValue());
    EXPECT_EQ(f.GetMarker(),      f_clone->GetMarker());
    EXPECT_EQ(f.GetTypeName(),    f_clone->GetTypeName());
    EXPECT_EQ(f.GetTotalLength(), f_clone->GetTotalLength());
}

}  // namespace huestream
