
subroutine generateQuakeML(quakemlstring, quakeml_length)

  implicit none
  character(len=*), intent(out) :: quakemlstring
  integer :: quakeml_length

  print *, "Generating QuakeML"

  quakemlstring = '<quakeml>\n<event unique_id=\"EV_01\">\n<location&
main=\"true\" unique_id=\"LOC_01\" analysis-type=\"M\">\n<origin-date&
timezone=\"00:00\">\n<year\>2004</year>\n<month\>09</month\n<day\>28</day>\n<hour\>17</hour>\n<minute\>15</minute>\n<seconds\>24.0</seconds>\n</origin-date>\n<latitude&
error="0">35.8</latitude>\n<longitude error="0">-120.4</longitude>\n<depth&
unit="km" error="0">7</depth>\n<magnitude unit="M"&
error="0">6.0</magnitude>\n<region>CENTRAL&
CALIFORNIA</region>\n<author>SPECFEM3D_GLOBE</author>\n</location>\n</event>\n</quakeml>'

end subroutine generateQuakeML

subroutine generateStationXMLstrings(stationxmlstrings, stationxmllengths)

  implicit none
  character(len=*) :: stationxmlstrings
  integer :: stationxmllengths

  print *, "Generating StationXML strings"

  stationxmlstrings = '<stationXML>\n<url>http://www.iris.edu/ws/station/query?net=II\&sta=AAK\&&
chan=BHZ,BHN,BHE,BH1,BH2\&loc=00,--\&level=chan</url>\n<refreshInterval>&
\n<unit>DAY</unit>\n<value>10</value>\n</refreshInterval>\n</stationXML>'

end subroutine generateStationXMLstrings
