/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <device.h>
#include <devicetree.h>
#include <init.h>
#include <kernel.h>
#include <drivers/sensor.h>
#include <bluetooth/services/bas.h>

#include <logging/log.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/event_manager.h>
#include <zmk/battery.h>
#include <zmk/events/battery_state_changed.h>

static uint8_t last_state_of_charge = 0;

uint8_t zmk_battery_state_of_charge() { return last_state_of_charge; }

#if DT_HAS_CHOSEN(zmk_battery)
static const struct device *const battery = DEVICE_DT_GET(DT_CHOSEN(zmk_battery));
#else
#warning                                                                                           \
    "Using a node labeled BATTERY for the battery sensor is deprecated. Set a zmk,battery chosen node instead. (Ignore this if you don't have a battery sensor.)"
static const struct device *battery;
#endif

static int zmk_battery_update(const struct device *battery) {
    struct sensor_value state_of_charge,
                        state_of_health,
                        voltage,
                        current,
                        current_standby,
                        current_max_load,
                        full_charge_capacity,
                        remaining_charge_capacity,
                        nominal_available_capacity,
                        full_available_capacity,
                        avg_power,
                        int_temp;

    int rc = 0;
    int status = 0;
    int rv = 0;

    //status = sensor_sample_fetch(battery);
    //status = sensor_sample_fetch_chan(battery, SENSOR_CHAN_GAUGE_STATE_OF_CHARGE);
    //LOG_WRN("-> Status: %d", status);


    struct zmk_battery_state_changed bat_state_data;

    /* ---------- get channel data for State of Charge ---------- */
    status = sensor_sample_fetch_chan(battery, SENSOR_CHAN_GAUGE_STATE_OF_CHARGE);

    if (status != 0 && status != -ENOTSUP) {
        LOG_WRN("Failed to get battery state of charge: %d", rc);
        rv++;
    }

    if (status == -ENOTSUP) {
        LOG_DBG("The battery sensor does not support the channel: State of Charge");
    } else {
        /* channel is supported by sensor */
        status = sensor_channel_get(battery, SENSOR_CHAN_GAUGE_STATE_OF_CHARGE, &state_of_charge);
        /* ensure channel value has actually changed before updating it */
        if (last_state_of_charge != state_of_charge.val1) {
            last_state_of_charge = state_of_charge.val1;

            LOG_DBG("Setting BAS GATT battery level to %d%%", last_state_of_charge);

            /* set BAS GATT battery level so it can be displayed at the host */
            rc = bt_bas_set_battery_level(last_state_of_charge);

            if (rc != 0) {
                LOG_WRN("Failed to set BAS GATT battery level (err %d)", rc);
                rv++;
            }

            /* append this value to the battery state changed event */
            bat_state_data.state_of_charge = last_state_of_charge;
            LOG_DBG("-----> battery State of Charge: %d%%", last_state_of_charge);

        }
    }

    /* ---------- get channel data for State of Health ---------- */
    status = sensor_sample_fetch_chan(battery, SENSOR_CHAN_GAUGE_STATE_OF_HEALTH);

    if (status != 0 && status != -ENOTSUP) {
        LOG_DBG("Failed to get battery State of Health: %d", status);
        rv++;
    }
    if (status == -ENOTSUP) {
        LOG_DBG("The configured battery sensor does not support the channel: State of Health");
    } else {
        status = sensor_channel_get(battery, SENSOR_CHAN_GAUGE_STATE_OF_HEALTH, &state_of_health);
        /* append this value to the battery state changed event */
        bat_state_data.state_of_health = state_of_health.val1;
        LOG_DBG("-----> battery State of Health: %d%%", state_of_health.val1);
    }

    /* ---------- get channel data for Voltage ---------- */
    status = sensor_sample_fetch_chan(battery, SENSOR_CHAN_GAUGE_VOLTAGE);

    if (status != 0 && status != -ENOTSUP) {
        LOG_DBG("Failed to get battery voltage: %d", status);
        rv++;
    }
    if (status == -ENOTSUP) {
        LOG_DBG("The configured battery sensor does not support the channel: Voltage");
    } else {
        status = sensor_channel_get(battery, SENSOR_CHAN_GAUGE_VOLTAGE, &voltage);
        /* append this value to the battery state changed event */
        bat_state_data.voltage = voltage.val1; /*combine val1 and val2 here*/
        LOG_DBG("-----> battery voltage: %d.%dV", voltage.val1, voltage.val2);
    }

    /* ---------- get channel data for Average Current ---------- */
    status = sensor_sample_fetch_chan(battery, SENSOR_CHAN_GAUGE_AVG_CURRENT);

    if (status != 0 && status != -ENOTSUP) {
        LOG_DBG("Failed to get battery Average Current: %d", status);
        rv++;
    }
    if (status == -ENOTSUP) {
        LOG_DBG("The battery sensor does not support the channel: Average Current");
    } else {
        status = sensor_channel_get(battery, SENSOR_CHAN_GAUGE_AVG_CURRENT, &current);
        /* append this value to the battery state changed event */
        bat_state_data.current = current.val1; /*combine val1 and val2 here*/
        LOG_DBG("-----> battery Average Current: %d.%dA", current.val1, current.val2);
    }

    /* ---------- get channel data for Standby Current ---------- */
    status = sensor_sample_fetch_chan(battery, SENSOR_CHAN_GAUGE_STDBY_CURRENT);

    if (status != 0 && status != -ENOTSUP) {
        LOG_DBG("Failed to get battery Standby Current: %d", status);
        rv++;
    }
    if (status == -ENOTSUP) {
        LOG_DBG("The battery sensor does not support the channel: Standby Current");
    } else {
        status = sensor_channel_get(battery, SENSOR_CHAN_GAUGE_STDBY_CURRENT, &current_standby);
        /* append this value to the battery state changed event */
        bat_state_data.current_standby = current_standby.val1; /*combine val1 and val2 here*/
        LOG_DBG("-----> battery Standby Current: %d.%dA", current_standby.val1,
                current_standby.val2);
    }

    /* ---------- get channel data for Maximum Load Current ---------- */
    status = sensor_sample_fetch_chan(battery, SENSOR_CHAN_GAUGE_MAX_LOAD_CURRENT);

    if (status != 0 && status != -ENOTSUP) {
        LOG_DBG("Failed to get battery Maximum Load Current: %d", status);
        rv++;
    }
    if (status == -ENOTSUP) {
        LOG_DBG("The battery sensor does not support the channel: Maximum Load Current");
    } else {
        status = sensor_channel_get(battery, SENSOR_CHAN_GAUGE_MAX_LOAD_CURRENT, &current_max_load);
        /* append this value to the battery state changed event */
        bat_state_data.current_max_load = current_max_load.val1; /*combine val1 and val2 here*/
        LOG_DBG("-----> battery Maximum Load Current: %d.%dA", current_max_load.val1,
                current_max_load.val2);
    }

    /* ---------- get channel data for Full Charge Capacity ---------- */
    status = sensor_sample_fetch_chan(battery, SENSOR_CHAN_GAUGE_FULL_CHARGE_CAPACITY);

    if (status != 0 && status != -ENOTSUP) {
        LOG_DBG("Failed to get battery Full Charge Capacity: %d", status);
        rv++;
    }
    if (status == -ENOTSUP) {
        LOG_DBG("The battery sensor does not support the channel: Full Charge Capacity");
    } else {
        status = sensor_channel_get(battery, SENSOR_CHAN_GAUGE_FULL_CHARGE_CAPACITY, &full_charge_capacity);
        /* append this value to the battery state changed event */
        bat_state_data.full_charge_capacity = full_charge_capacity.val1; /*combine val1 and val2 here*/
        LOG_DBG("-----> battery Full Charge Capacity: %d.%dmAh", full_charge_capacity.val1,
                full_charge_capacity.val2);
    }

    /* ---------- get channel data for Remaining Charge Capacity ---------- */
    status = sensor_sample_fetch_chan(battery, SENSOR_CHAN_GAUGE_REMAINING_CHARGE_CAPACITY);

    if (status != 0 && status != -ENOTSUP) {
        LOG_DBG("Failed to get battery Remaining Charge Capacity: %d", status);
        rv++;
    }
    if (status == -ENOTSUP) {
        LOG_DBG("The battery sensor does not support the channel: Remaining Charge Capacity");
    } else {
        status = sensor_channel_get(battery, SENSOR_CHAN_GAUGE_REMAINING_CHARGE_CAPACITY, &remaining_charge_capacity);
        /* append this value to the battery state changed event */
        bat_state_data.remaining_charge_capacity = remaining_charge_capacity.val1; /*combine val1 and val2 here*/
        LOG_DBG("-----> battery Remaining Charge Capacity: %d.%dmAh", remaining_charge_capacity.val1,
                remaining_charge_capacity.val2);
    }

    /* ---------- get channel data for Nominal Available Capacity ---------- */
    status = sensor_sample_fetch_chan(battery, SENSOR_CHAN_GAUGE_NOM_AVAIL_CAPACITY);

    if (status != 0 && status != -ENOTSUP) {
        LOG_DBG("Failed to get battery Nominal Available Capacity: %d", status);
        rv++;
    }
    if (status == -ENOTSUP) {
        LOG_DBG("The battery sensor does not support the channel: Nominal Available Capacity");
    } else {
        status = sensor_channel_get(battery, SENSOR_CHAN_GAUGE_NOM_AVAIL_CAPACITY, &nominal_available_capacity);
        /* append this value to the battery state changed event */
        bat_state_data.nominal_available_capacity = nominal_available_capacity.val1; /*combine val1 and val2 here*/
        LOG_DBG("-----> battery Nominal Available Capacity: %d.%dmAh", nominal_available_capacity.val1,
                nominal_available_capacity.val2);
    }

    /* ---------- get channel data for Full Available Capacity ---------- */
    status = sensor_sample_fetch_chan(battery, SENSOR_CHAN_GAUGE_FULL_AVAIL_CAPACITY);

    if (status != 0 && status != -ENOTSUP) {
        LOG_DBG("Failed to get battery Full Available Capacity: %d", status);
        rv++;
    }
    if (status == -ENOTSUP) {
        LOG_DBG("The battery sensor does not support the channel: Full Available Capacity");
    } else {
        status = sensor_channel_get(battery, SENSOR_CHAN_GAUGE_FULL_AVAIL_CAPACITY, &full_available_capacity);
        /* append this value to the battery state changed event */
        bat_state_data.full_available_capacity = full_available_capacity.val1; /*combine val1 and val2 here*/
        LOG_DBG("-----> battery Full Available Capacity: %d.%dmAh", full_available_capacity.val1,
                full_available_capacity.val2);
    }

    /* ---------- get channel data for Average Power Usage ---------- */
    status = sensor_sample_fetch_chan(battery, SENSOR_CHAN_GAUGE_AVG_POWER);

    if (status != 0 && status != -ENOTSUP) {
        LOG_DBG("Failed to get battery Average Power Usage: %d", status);
        rv++;
    }
    if (status == -ENOTSUP) {
        LOG_DBG("The battery sensor does not support the channel: Average Power Usage");
    } else {
        status = sensor_channel_get(battery, SENSOR_CHAN_GAUGE_AVG_POWER, &avg_power);
        /* append this value to the battery state changed event */
        bat_state_data.avg_power = avg_power.val1; /*combine val1 and val2 here*/
        LOG_DBG("-----> battery Average Power Usage: %d.%dmW", avg_power.val1, avg_power.val2);
    }

    /* ---------- get channel data for Internal IC Temperature ---------- */
    status = sensor_sample_fetch_chan(battery, SENSOR_CHAN_GAUGE_TEMP);

    if (status != 0 && status != -ENOTSUP) {
        LOG_DBG("Failed to get Internal IC Temperature: %d", status);
        rv++;
    }
    if (status == -ENOTSUP) {
        LOG_DBG("The battery sensor does not support the channel: Internal IC Temperature");
    } else {
        status = sensor_channel_get(battery, SENSOR_CHAN_GAUGE_TEMP, &int_temp);
        /* append this value to the battery state changed event */
        bat_state_data.int_temp = int_temp.val1; /*combine val1 and val2 here*/
        LOG_DBG("-----> battery Internal IC Temperature: %d.%d C", int_temp.val1, int_temp.val2);
    }

    /* trigger event to notify of battery sensor value changes */
    ZMK_EVENT_RAISE(new_zmk_battery_state_changed(bat_state_data));

    return rv;

} /* zmk_battery_update() */

static void zmk_battery_work(struct k_work *work) {
    int rc = zmk_battery_update(battery);

    if (rc != 0) {
        LOG_DBG("Failed to update battery value: %d.", rc);
    }
}

K_WORK_DEFINE(battery_work, zmk_battery_work);

static void zmk_battery_timer(struct k_timer *timer) { k_work_submit(&battery_work); }

K_TIMER_DEFINE(battery_timer, zmk_battery_timer, NULL);

static int zmk_battery_init(const struct device *_arg) {
#if !DT_HAS_CHOSEN(zmk_battery)

    battery = device_get_binding("BATTERY");

    if (battery == NULL) {
        return -ENODEV;
    }

    LOG_WRN("Finding battery device labeled BATTERY is deprecated. Use zmk,battery chosen node.");
#endif

    if (!device_is_ready(battery)) {
        LOG_ERR("Battery device \"%s\" is not ready", battery->name);
        return -ENODEV;
    }

    LOG_DBG("--- Device labelled 'BATTERY' found! ---");

    int rc = zmk_battery_update(battery);

    if (rc != 0) {
        LOG_ERR("Failed to update %d battery value(s) which the configured sensor supports.", rc);
        return rc;
    }

    k_timer_start(&battery_timer, K_MINUTES(1), K_SECONDS(CONFIG_ZMK_BATTERY_REPORT_INTERVAL));

    return 0;
}

SYS_INIT(zmk_battery_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
