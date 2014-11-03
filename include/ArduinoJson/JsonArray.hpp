// Copyright Benoit Blanchon 2014
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "Internals/ReferenceType.hpp"
#include "JsonArrayConstIterator.hpp"
#include "JsonArrayIterator.hpp"
#include "JsonPrintable.hpp"

#define JSON_ARRAY_SIZE(NUMBER_OF_ELEMENTS) \
  (sizeof(JsonArray) + (NUMBER_OF_ELEMENTS) * sizeof(Internals::JsonArrayNode))

namespace ArduinoJson {

class JsonObject;
class JsonBuffer;

class JsonArray : public JsonPrintable<JsonArray>,
                  public Internals::ReferenceType {
  friend class JsonBuffer;

 public:
  typedef JsonValue value_type;
  typedef JsonArrayIterator iterator;
  typedef JsonArrayConstIterator const_iterator;

  int size() const;

  bool success() { return _buffer != NULL; }

  value_type &operator[](int index) const { return at(index); }
  value_type &at(int index) const;

  template <typename T>
  void add(T value) {
    add().set(value);
  }

  value_type &add();
  void add(double value, uint8_t decimals) { add().set(value, decimals); }
  void add(JsonArray &nestedArray) { add().set(nestedArray); }
  void add(JsonObject &nestedObject) { add().set(nestedObject); }

  JsonArray &createNestedArray();
  JsonObject &createNestedObject();

  iterator begin() { return iterator(_firstNode); }
  iterator end() { return iterator(NULL); }

  const_iterator begin() const { return const_iterator(_firstNode); }
  const_iterator end() const { return const_iterator(NULL); }

  static JsonArray &invalid() { return _invalid; }

  template <typename T>
  void writeTo(T &writer) const;

 private:
  // constructor is private: instance must be created via a JsonBuffer
  JsonArray(JsonBuffer *buffer) : _buffer(buffer), _firstNode(NULL) {}

  Internals::JsonArrayNode *createNode();
  inline void addNode(Internals::JsonArrayNode *node);

  JsonBuffer *_buffer;
  Internals::JsonArrayNode *_firstNode;
  static JsonArray _invalid;
};
}