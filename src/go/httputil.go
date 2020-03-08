// Copyright © 2019 ichenq@outlook.com All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.

package netutil

import (
	"bytes"
	"compress/zlib"
	"fatchoy/x/strutil"
	"fmt"
	"net"
	"reflect"
	"strings"

	"github.com/golang/protobuf/jsonpb"
	pb "github.com/golang/protobuf/proto"
	log "github.com/sirupsen/logrus"
	"github.com/valyala/fasthttp"
)

// name should be ASCII only
func ValidateHttpQueryName(s string) string {
	if s == "" {
		return s
	}
	var name = strings.ToLower(s)
	for i := 0; i < len(name); i++ {
		var r = name[i]
		switch {
		case ('a' <= r && r <= 'z') || ('A' <= r && r <= 'Z') || ('0' <= r && r <= '9'):
			break
		case r == '_' || r == '-':
			break
		default:
			return ""
		}
	}
	return name
}

// Remote IP address of request
func RemoteIPAddress(ctx *fasthttp.RequestCtx) string {
	var ip string
	// test if under reverse proxy
	var text = strutil.FastBytesToString(ctx.Request.Header.Peek("X-Forwarded-For"))
	if text != "" {
		var idx = strings.Index(text, ",")
		if idx <= 0 {
			ip = text
		} else {
			ip = text[:idx] // first item
		}
	}
	if ip == "" {
		text = strutil.FastBytesToString(ctx.Request.Header.Peek("X-Real-IP"))
		if text != "" {
			ip = text
		}
	}
	if ip == "" {
		ip, _, _ = net.SplitHostPort(ctx.RemoteAddr().String())
	}
	return ip
}

// deflate compression
func deflateResponse(buf *bytes.Buffer, data []byte, acceptEncoding string) error {
	if n := strings.Index(acceptEncoding, "deflate"); n < 0 {
		return nil
	}
	var wr = zlib.NewWriter(buf)
	if _, err := wr.Write(data); err != nil {
		return err
	}
	if err := wr.Close(); err != nil {
		return err
	}
	return nil
}

// 从HTTP请求中解码proto
func DecodeProtoHttpBody(ctx *fasthttp.RequestCtx, msg pb.Message) error {
	var contentType = string(ctx.Request.Header.Peek("Content-Type"))
	if contentType == "" {
		contentType = "application/octet-stream"
	}

	switch contentType {
	case "application/json":
		var reader = bytes.NewReader(ctx.PostBody())
		return jsonpb.Unmarshal(reader, msg)

	case "application/octet-stream", "application/binary":
		if err := pb.Unmarshal(ctx.PostBody(), msg); err != nil {
			return err
		}
		return nil

	default:
		return fmt.Errorf("unsupported content type %s", contentType)
	}
}

// 将proto消息编码到HTTP响应
func WriteProtoHttpResponse(ctx *fasthttp.RequestCtx, msg pb.Message, contentType string) error {
	if msg == nil || reflect.ValueOf(msg).IsNil() {
		return nil
	}
	switch contentType {
	case "application/json":
		var m jsonpb.Marshaler
		var buf bytes.Buffer
		if err := m.Marshal(&buf, msg); err != nil {
			return err
		}
		ctx.Response.Header.Set("Content-Type", "application/json")
		ctx.SetBody(buf.Bytes())

	default:
		rawbytes, err := pb.Marshal(msg)
		if err != nil {
			log.Errorf("WriteProtoResponse: pb.Marshal: %v", err)
			return err
		}
		ctx.Response.Header.Set("Content-Type", "application/octet-stream")
		ctx.SetBody(rawbytes)
	}
	return nil
}

type Decorator func(fasthttp.RequestHandler) fasthttp.RequestHandler

func Decorate(f fasthttp.RequestHandler, ds ...Decorator) fasthttp.RequestHandler {
	decorated := f
	for _, decorate := range ds {
		decorated = decorate(decorated)
	}
	return func(ctx *fasthttp.RequestCtx) {
		decorated(ctx)
	}
}
