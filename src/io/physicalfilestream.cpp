#include "physicalfilestream.h"

#include <algorithm>

namespace Impacto {
namespace Io {

PhysicalFileStream::~PhysicalFileStream() { SDL_RWclose(RW); }

IoError PhysicalFileStream::Create(std::string const& fileName,
                                   InputStream** out) {
  SDL_RWops* rw = SDL_RWFromFile(fileName.c_str(), "rb");
  if (!rw) return IoError_Fail;
  int64_t size = SDL_RWsize(rw);
  if (size <= 0) return IoError_Fail;
  PhysicalFileStream* result = new PhysicalFileStream;
  result->RW = rw;
  result->Meta.Size = size;
  result->SourceFileName = fileName;
  result->Meta.FileName = fileName;
  *out = (InputStream*)result;
  return IoError_OK;
}

IoError PhysicalFileStream::FillBuffer() {
  int64_t read = SDL_RWread(RW, Buffer, 1, PhysicalBufferSize);
  if (read < 0) return IoError_Fail;
  BufferFill = read;
  return IoError_OK;
}

int64_t PhysicalFileStream::Read(void* buffer, int64_t sz) {
  return ReadBuffered(buffer, sz);
}

int64_t PhysicalFileStream::Seek(int64_t offset, int origin) {
  int64_t absPos;
  if (origin == RW_SEEK_SET) {
    absPos = offset;
  } else if (origin == RW_SEEK_CUR) {
    absPos = Position + offset;
  } else if (origin == RW_SEEK_END) {
    absPos = Meta.Size - offset;
  }
  if (absPos < 0 || absPos > Meta.Size) return IoError_Fail;

  int64_t err = SeekBuffered(absPos);
  if (err < IoError_OK) {
    BufferFill = 0;
    BufferConsumed = 0;
    err = SDL_RWseek(RW, offset, origin);
    if (err < 0) return IoError_Fail;
    Position = err;
  }
  return err;
}

IoError PhysicalFileStream::Duplicate(InputStream** outStream) {
  SDL_RWops* rw = SDL_RWFromFile(SourceFileName.c_str(), "rb");
  if (!rw) return IoError_Fail;
  int64_t filePos = SDL_RWtell(RW);
  if (filePos < 0) {
    SDL_RWclose(rw);
    return IoError_Fail;
  }
  int64_t pos = SDL_RWseek(rw, filePos, RW_SEEK_SET);
  if (pos != filePos) {
    SDL_RWclose(rw);
    return IoError_Fail;
  }
  PhysicalFileStream* result = new PhysicalFileStream(*this);
  result->RW = rw;
  *outStream = (InputStream*)result;
  return IoError_OK;
}

}  // namespace Io
}  // namespace Impacto