// The MIT License (MIT)
//
// Copyright (c) 2011-2020 Christopher M. Poole <mail@christopherpoole.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// CADMesh //
#include "BuiltInReader.hh"
#include "STLReader.hh"
#include "OBJReader.hh"
#include "PLYReader.hh"


namespace CADMesh {

    namespace File {

        G4bool BuiltInReader::Read(G4String filepath) {
            File::Reader *reader = nullptr;

            auto type = TypeFromName(filepath);

            if (type == STL) {
                reader = new File::STLReader();
            } else if (type == OBJ) {
                reader = new File::OBJReader();
            } else if (type == PLY) {
                reader = new File::PLYReader();
            } else {
                Exceptions::ReaderCantReadError("BuildInReader::Read", type, filepath);
            }


            if (!reader->Read(filepath)) {
                return false;
            }

            SetMeshes(reader->GetMeshes());
            return true;
        }


        G4bool BuiltInReader::CanRead(Type type) {
            return type == STL || type == OBJ || type == PLY;
        }


        std::shared_ptr <BuiltInReader> BuiltIn() {
            return std::make_shared<BuiltInReader>();
        }

    } // File namespace

} // CADMesh namespace

