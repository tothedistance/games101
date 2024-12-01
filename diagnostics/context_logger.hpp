#if !defined(CONTEXT_LOGGER_H)
#define CONTEXT_LOGGER_H

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>

#include "eigen3/Eigen/Eigen"
#include "rapidjson/document.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/pointer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/rapidjson.h"

namespace games {

// #define OPEN_LOGGER
#if defined(OPEN_LOGGER)
class ContextLogger {
    std::filesystem::path filepath;
    rapidjson::Document document;
    std::string cur_key{'/'};

public:
    ContextLogger() {
        document.SetObject();
        filepath = std::filesystem::current_path() / "context.json";
    }

    ContextLogger(const std::string& name) {
        document.SetObject();
        filepath = std::filesystem::current_path() / name;
        cur_key = '/' + name;
    }

    ContextLogger(const ContextLogger&) = delete;
    ContextLogger& operator=(const ContextLogger&) = delete;
    ContextLogger(ContextLogger&& rhs) {
        document = std::move(rhs.document);
        filepath = std::move(rhs.filepath);
        cur_key = std::move(rhs.cur_key);
    }

    ContextLogger& operator=(ContextLogger&& rhs) {
        document = std::move(rhs.document);
        filepath = std::move(rhs.filepath);
        cur_key = std::move(rhs.cur_key);
        return *this;
    }

    ~ContextLogger() {
    }

    void flush() {
        std::ofstream ofs(filepath);
        rapidjson::OStreamWrapper osw(ofs);
        rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
        document.Accept(writer);
    }

    rapidjson::Document::AllocatorType& get_allocator() {
        return document.GetAllocator();
    }

    std::string get_key() {
        return cur_key;
    }

    void set_key(const std::string& key) {
        cur_key = key;
        return;
    }

    void backspace() {
        decltype(cur_key.back()) ch = cur_key.back();
        while (ch != '#') {
            if (ch == '/') {
                cur_key.pop_back();
                break;
            }
            cur_key.pop_back();
            ch = cur_key.back();
        }
    }

    void append_key(const std::string& key) {
        cur_key = cur_key + '/' + key;
        return;
    }

    template <typename _Scalar, int _Rows, int _Cols>
    void log(Eigen::Matrix<_Scalar, _Rows, _Cols> m) {
        rapidjson::Value v(rapidjson::kArrayType);
        for (int i = 0; i < m.rows(); i++) {
            rapidjson::Value row(rapidjson::kArrayType);
            for (int j = 0; j < m.cols(); j++) {
                row.PushBack(m(i, j), document.GetAllocator());
            }
            v.PushBack(row, document.GetAllocator());
        }
        rapidjson::Pointer(cur_key.c_str()).Set(document, v);
        return;
    }

    template <typename _Scalar, int _Rows, int _Cols>
    void push_back(Eigen::Matrix<_Scalar, _Rows, _Cols> m) {
        rapidjson::Value v(rapidjson::kArrayType);
        for (int i = 0; i < m.rows(); i++) {
            rapidjson::Value row(rapidjson::kArrayType);
            for (int j = 0; j < m.cols(); j++) {
                row.PushBack(m(i, j), document.GetAllocator());
            }
            v.PushBack(row, document.GetAllocator());
        }
        rapidjson::Value array(rapidjson::kArrayType);
        array.PushBack(v, document.GetAllocator());
        rapidjson::Pointer(cur_key.c_str()).Set(document, array);
        return;
    }

    void log(rapidjson::Value&& value) {
        rapidjson::Pointer(cur_key.c_str()).Set(document, value);
        return;
    }

    void log(rapidjson::Pointer key, rapidjson::Value&& value) {
        key.Set(document, value);
        return;
    }

    friend class ContextLoggerFactory;
};

#else  // OPEN_LOGGER
class ContextLogger {
    std::filesystem::path filepath;
    rapidjson::Document document;
    std::string cur_key{'/'};

public:
    ContextLogger() {
        document.SetObject();
        filepath = std::filesystem::current_path() / "context.json";
    }

    ContextLogger(const std::string& name) {
        document.SetObject();
        filepath = std::filesystem::current_path() / name;
        cur_key = '/' + name;
    }

    ContextLogger(const ContextLogger&) = delete;
    ContextLogger& operator=(const ContextLogger&) = delete;
    ContextLogger(ContextLogger&& rhs) {
        document = std::move(rhs.document);
        filepath = std::move(rhs.filepath);
        cur_key = std::move(rhs.cur_key);
    }

    ContextLogger& operator=(ContextLogger&& rhs) {
        document = std::move(rhs.document);
        filepath = std::move(rhs.filepath);
        cur_key = std::move(rhs.cur_key);
        return *this;
    }

    ~ContextLogger() {
    }

    void flush() {
        std::ofstream ofs(filepath);
        rapidjson::OStreamWrapper osw(ofs);
        rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
        document.Accept(writer);
    }

    rapidjson::Document::AllocatorType& get_allocator() {
        return document.GetAllocator();
    }

    std::string get_key() {
        return cur_key;
    }

    void set_key(const std::string& key) {
        cur_key = key;
        return;
    }

    void backspace() {
        decltype(cur_key.back()) ch = cur_key.back();
        while (ch != '#') {
            if (ch == '/') {
                cur_key.pop_back();
                break;
            }
            cur_key.pop_back();
            ch = cur_key.back();
        }
    }

    void append_key(const std::string& key) {
        cur_key = cur_key + '/' + key;
        return;
    }

    template <typename _Scalar, int _Rows, int _Cols>
    void log(Eigen::Matrix<_Scalar, _Rows, _Cols> m) {
        return;
    }

    template <typename _Scalar, int _Rows, int _Cols>
    void push_back(Eigen::Matrix<_Scalar, _Rows, _Cols> m) {
        return;
    }

    void log(rapidjson::Value&& value) {
        return;
    }

    void log(rapidjson::Pointer key, rapidjson::Value&& value) {
        return;
    }

    friend class ContextLoggerFactory;
};

#endif

class ContextLoggerFactory {
public:
    static ContextLoggerFactory& get_instance() {
        return instance;
    }
    ContextLogger& get_by_name(const std::string& name) {
        if (contexts.find(name) == contexts.end()) {
            return create(name);
        } else {
            return contexts[name];
        }
    }
    ContextLogger& create(const std::string& name) {
        contexts.insert({name, std::move(ContextLogger(name))});
        return contexts.at(name);
    }

private:
    static std::unordered_map<std::string, ContextLogger> contexts;

private:
    ContextLoggerFactory() = default;
    ContextLoggerFactory(const ContextLoggerFactory&) = delete;
    ContextLoggerFactory& operator=(const ContextLoggerFactory&) = delete;

    static ContextLoggerFactory instance;
};

}  // namespace games

#endif  // CONTEXT_LOGGER_H
