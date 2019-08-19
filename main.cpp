
// Copyright (c) 2019 The Karbowanec developers
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Code formatting based on CS106B Style

#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[]) {

  QCoreApplication::setOrganizationName("Karbowanec");
  QCoreApplication::setOrganizationDomain("karbo.io");
  QCoreApplication::setApplicationName("Launcher");

  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
