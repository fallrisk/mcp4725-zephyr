.. _mcp4725-zephyr:

MCP4725 Zephyr
##############

Overview
********
Example of using I2C on an Arduino Due to control an MCP4725 digital-to-analog
converter. This example is with the Zephyr OS.

Wiring
******

Arduino Due ---- MCP4725
TWCK0 10 PWMH ---- SCL
TWD0 9 PWMH ---- SDA
GND ---- A0
GND ---- Vss
5V  ---- Vdd

Building and Running
********************

This project outputs a square wave on the Vout pin of the MCP4725. To make the
project enter the following:

.. code-block:: console

   $ cd mcp4725-zephyr
   $ make

Sample Output
=============

You should see a square wave on the output pin of the MCP4725.
