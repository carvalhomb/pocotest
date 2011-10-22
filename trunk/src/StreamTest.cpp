#include <sstream>

#include <Poco/Base64Decoder.h>
#include <Poco/Base64Encoder.h>
#include <Poco/HexBinaryDecoder.h>
#include <Poco/HexBinaryEncoder.h>

#include <gtest/gtest.h>

TEST(StreamTest, HexBinaryDecoderTest)
{
	std::istringstream iss("30313233343536373839");
	Poco::HexBinaryDecoder decoder(iss);

	std::string rs;
	decoder >> rs;

	EXPECT_EQ("0123456789", rs);
}

TEST(StreamTest, HexBinaryEncoderTest)
{
	std::ostringstream oss;
	Poco::HexBinaryEncoder encoder(oss);

	encoder << "9876543210ABCDEFG";

	std::string rs = oss.str();
	EXPECT_EQ("3938373635343332313041424344454647", rs);
}

TEST(StreamTest, Base64DecoderTest)
{
	std::istringstream iss("TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=");
	Poco::Base64Decoder decoder(iss);

	char temp[1024];
	decoder.read(temp, 1024);
	std::string result(temp, decoder.gcount());

	std::string expectedStr = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";
	EXPECT_EQ(expectedStr, result);
}

TEST(StreamTest, Base64EncoderTest)
{
	std::ostringstream oss;
	Poco::Base64Encoder encoder(oss);

	encoder << "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";
	std::string rs = oss.str();

	EXPECT_FALSE(rs.empty());
}
