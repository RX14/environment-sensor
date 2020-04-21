require "socket"
require "json"

tty = ARGV[0]
statsd_host = ARGV[1]
statsd_tag = ARGV[2]

`stty -F #{tty} raw 115200`

serial = File.open(tty)
serial.blocking = false

struct Data
  getter temperature : Float64
  getter pressure : Float64
  getter humidity : Float64
  getter co2 : Float64

  include JSON::Serializable
end

statsd = TCPSocket.new(statsd_host, 8125)

serial.each_line do |line|
  json = Data.from_json(line)

  statsd << "#{statsd_tag}.temperature:#{json.temperature}|g\n"
  statsd << "#{statsd_tag}.pressure:#{json.pressure}|g\n"
  statsd << "#{statsd_tag}.humidity:#{json.humidity}|g\n"
  statsd << "#{statsd_tag}.co2:#{json.co2}|g\n"
rescue JSON::ParseException
  STDERR.puts line
end
