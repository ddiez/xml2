#include <Rcpp.h>
using namespace Rcpp;

#include <libxml/tree.h>
#include <libxml/HTMLtree.h>

#include "xml2_types.h"
#include "xml2_utils.h"

// [[Rcpp::export]]
void doc_write_xml(XPtrDoc x, std::string path, bool format) {
  int res = xmlSaveFormatFile(
    R_ExpandFileName(path.c_str()),
    x.checked_get(),
    format ? 1 : 0
  );
  if (res == -1)
    Rcpp::stop("Failed to write to %s", path);
}
// [[Rcpp::export]]
void doc_write_html(XPtrDoc x, std::string path, bool format) {
  int res = htmlSaveFileFormat(
    R_ExpandFileName(path.c_str()),
    x.checked_get(),
    "UTF-8",
    format ? 1 : 0
  );
  if (res == -1)
    Rcpp::stop("Failed to write to %s", path);
}


// [[Rcpp::export]]
void node_write_xml(XPtrNode n, XPtrDoc d, std::string path) {
  FILE* f = fopen(path.c_str(), "wb");
  xmlElemDump(f, d.checked_get(), n.checked_get());
  fclose(f);
}
// [[Rcpp::export]]
void node_write_html(XPtrNode n, XPtrDoc d, std::string path) {
  FILE* f = fopen(path.c_str(), "wb");
  htmlNodeDumpFile(f, d.checked_get(), n.checked_get());
  fclose(f);
}


// [[Rcpp::export]]
CharacterVector doc_format_xml(XPtrDoc x) {
  xmlChar *s;
  xmlDocDumpMemory(x.checked_get(), &s, NULL);

  return Xml2String(s).asRString();
}
// [[Rcpp::export]]
CharacterVector doc_format_html(XPtrDoc x) {
  xmlChar *s;
  htmlDocDumpMemory(x.checked_get(), &s, NULL);

  return Xml2String(s).asRString();
}


// [[Rcpp::export]]
CharacterVector node_format_xml(XPtrDoc doc, XPtrNode node,
                                bool format = true,
                                int indent = 0) {
  boost::shared_ptr<xmlBuffer> buffer(xmlBufferCreate(), xmlFree);
  xmlNodeDump(buffer.get(), doc.checked_get(), node.checked_get(), indent, format);

  return Xml2String(buffer->content).asRString();
}
// [[Rcpp::export]]
CharacterVector node_format_html(XPtrDoc doc, XPtrNode node,
                                 bool format = true) {
  boost::shared_ptr<xmlBuffer> buffer(xmlBufferCreate(), xmlBufferFree);
  xmlOutputBuffer *outputBuffer = xmlOutputBufferCreateBuffer( buffer.get(), NULL );

  htmlNodeDumpFormatOutput(
    outputBuffer,
    doc.checked_get(),
    node.checked_get(),
    NULL,
    format ? 1 : 0
  );

  return Xml2String(buffer->content).asRString();
}
